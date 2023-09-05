#include <QRandomGenerator>  //Included for Random Number used in creating textures for Seis Gathers
#include <algorithm>
#include <QScreen>
#include <QtGlobal>
#include "cGLWindow.h"
#include "cerrwarninfo.h"

#define QT_DEBUG

//namespace GnV //GnGViewer
//{

GLenum cGLWindow::glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        QString error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }

        //std::cout << error << " | " << file << " (" << line << ")" << std::endl;
        QString s = error + " | " + file + "(" + QString::number(line) + ")";
        cErrWarnInfo::EWI(ewiINFOR, s );

    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

    /// <summary>
    /// Provides OpenGL Debug Information.
    /// Details at https://www.kdab.com/opengl-in-qt-5-1-part-4/
    /// and 
    /// https://doc.qt.io/qt-5/qopengldebuglogger.html#details
    /// </summary>
    /// <param name="parent"></param>
//cGLWindow::GlDebug()
//{}
 
    cGLWindow::cGLWindow(QWindow* parent) :
        m_mouseFirst(true),
        m_isProjPresp(true),
        m_aspect(0.0),
        m_v3CrdNorm(0.0,0.0,0.0),
        m_v3GlobalAvgEstElvNrth(0.0, 0.0, 0.0),
        m_rotationAngel(30), //30 was the number used in development so using 30 as initial
        //m_fov(25.0),
        m_sclDpth(5.0),
        m_minEasting(std::numeric_limits<double>::max()),
        m_maxEasting(-std::numeric_limits<double>::max()),
        m_minNorthing(std::numeric_limits<double>::max()),
        m_maxNorthing(-std::numeric_limits<double>::max()),
        m_minElevation(std::numeric_limits<double>::max()),
        m_maxElevation(-std::numeric_limits<double>::max())
    {
        Q_UNUSED(parent);
        // Set OpenGL Version information
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setVersion(4, 4);

        // Setting Multisampling
        format.setSamples(16);

//#ifdef QT_DEBUG
//        format.setOption(QSurfaceFormat::DebugContext);
//#endif

        this->setFormat(format);
    }

    cGLWindow::~cGLWindow()
    {
        for (auto i : m_LstSeisXZYGths) //Destroy all Vertex Texture Objects
        {
            /// <summary>
            /// Attempts to delete a non-existent texture are simply ignored without any error
            /// So, calling glDeleteTextures is considered safe.
            /// </summary>
            glDeleteTextures(1, &i->m_glTextureID);
            glDeleteTextures(1, &i->m_glColoMapTextureID);
        }
    }

    /// Sets up the OpenGL resources and state. Gets called once before the first time resizeGL() or paintGL() is called.
   /// https://doc.qt.io/qt-5/qopenglwidget.html
    void cGLWindow::initializeGL()
    {
        initializeOpenGLFunctions();

        //// Code snippet taken from https://ferransole.wordpress.com/2014/06/09/array-textures/
        //Check if Opengl version is at least 3.0
        const GLubyte* glVersion(glGetString(GL_VERSION));
        int major = glVersion[0] - '0';
        int minor = glVersion[2] - '0';
        if (major < 3 || minor < 0)
        {
            cErrWarnInfo::EWI(ewiERROR, "Minimum OpenGL version required for this demo is 3.0. Your current version is " + QString::number(major) + "." + QString::number(minor));
            return;
        }
        //// END Code snippet taken from https://ferransole.wordpress.com/2014/06/09/array-textures/

//#ifdef QT_DEBUG
//        if (!m_GLlogger->initialize())
//            qDebug() << "UNABLE to initialize the OpenGlLogger";
//
//        glEnable(GL_DEBUG_OUTPUT);
//        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//
//        connect(m_GLlogger, &QOpenGLDebugLogger::messageLogged, this, &cGLWindow::GLLogHandler);
//        m_GLlogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
//#endif //QT_DEBUG

        // Setting up blending and anti aliasing, taken from RED GL Book page 180
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);// GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_MULTISAMPLE);
        // END Setting up blending and anti aliasing

        if (!InitShaders())
            return;

        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        printContextInformation();
    }

    bool cGLWindow::GLLogHandler(QOpenGLDebugMessage message)
    {
        qDebug() << message;

        return false;
    }

    /// <summary>
    /// This function loads, compiles and links the shaders with the m_program variable
    /// It also updates locations of some member with their corresponding shader uniforms and attributes
    /// </summary>
    /// <returns>Returns True if successful, otherwise False</returns>
    bool cGLWindow::InitShaders()
    {

        /// <summary>
        /// There are two compiled and linked GLSL programs.
        /// One for rendering header objects m_progHeader.
        /// Second for rendering seismic gather objects. Fragment shader in this m_progSeisGth program uses texture.
        /// </summary>
        /// <returns></returns>
        m_progHeader = std::make_shared<QOpenGLShaderProgram>(this);

        if (!m_progHeader->addShaderFromSourceCode(QOpenGLShader::Vertex, Shader_Vert_Headers))
        {
            cErrWarnInfo::EWI(ewiERROR, m_progHeader->log());  //If failed, print error and return.
            return false;
        }

        if (!m_progHeader->addShaderFromSourceCode(QOpenGLShader::Fragment, Shader_Frag_Headers))
        {
            cErrWarnInfo::EWI(ewiERROR, m_progHeader->log());  //If failed, print error and return.
            return false;
        }

        if (!m_progHeader->link())
        {
            cErrWarnInfo::EWI(ewiERROR, m_progHeader->log());  //If failed, print error and return.
            return false;
        }

  
        /// <summary>
        /// Compiling and linking m_progSeisGth shaders
        /// </summary>
        /// <returns></returns>
        m_progSeisGth = std::make_shared<QOpenGLShaderProgram>(this);

        if (!m_progSeisGth->addShaderFromSourceCode(QOpenGLShader::Vertex, Shader_Vert_SeisGth))
        {
            cErrWarnInfo::EWI(ewiERROR, m_progSeisGth->log());  //If failed, print error and return.
            return false;
        }

        if (!m_progSeisGth->addShaderFromSourceCode(QOpenGLShader::Fragment, Shader_Frag_SeisGth))
        {
            cErrWarnInfo::EWI(ewiERROR, m_progSeisGth->log());  //If failed, print error and return.
            return false;
        }

        if (!m_progSeisGth->link())
        {
            cErrWarnInfo::EWI(ewiERROR, m_progSeisGth->log());  //If failed, print error and return.
            return false;
        }


        /// <summary>
        /// Updating Program variable locations for Header program
        /// </summary>
        UpdateShaderVarLocs();

        /// ################# Initializing VISUAL REFERENCES
//        if (m_aSymbVertLocHdrProg != -1)
//            VAO_VisualRef(m_VaoRefGrid, m_aSymbVertLocHdrProg);

        return true;
    }

    bool cGLWindow::UpdateShaderVarLocs()
    {
        //########### Variables for the Header shader program
        m_aSymbVertLocHdrProg = m_progHeader->attributeLocation("v3inSymbVert");
        Q_ASSERT(m_aSymbVertLocHdrProg != -1);

        m_aSymbScalLocHdrProg = m_progHeader->attributeLocation("v3inSymScale");
        Q_ASSERT(m_aSymbScalLocHdrProg != -1);

        m_uModelLocHdrProg = m_progHeader->uniformLocation("uModel");
        Q_ASSERT(m_uModelLocHdrProg != -1);

        m_aColorLocHdrProg = m_progHeader->attributeLocation("v4inColor");
        Q_ASSERT(m_aColorLocHdrProg != -1);

        /// <summary>
        /// Locations used in the Seismic gather GLSL program
        /// </summary>
        m_aSymbVertLocGthProg = m_progSeisGth->attributeLocation("v3inSymbVert");
        Q_ASSERT(m_aSymbVertLocGthProg != -1);

        m_aSymbScalLocGthProg = m_progSeisGth->attributeLocation("v3inSymScale");
        Q_ASSERT(m_aSymbScalLocGthProg != -1);

        m_uModelLocGthProg = m_progSeisGth->uniformLocation("uModel");
        Q_ASSERT(m_uModelLocGthProg != -1);

        m_aTexCoordLocGthProg = m_progSeisGth->attributeLocation("v2inTexCoord");
        //Q_ASSERT(m_aTexCoordLocGthProg != -1);

        m_uLayerLocGthProg = m_progSeisGth->uniformLocation("intLayer");
        //Q_ASSERT(m_uLayerLocGthProg != -1);

        m_uTexSamLocGthProg = m_progSeisGth->uniformLocation("SamplesAry");
        //Q_ASSERT(m_uTexSamLocGthProg != -1);

        //m_uTexColrMapLocGthProg = m_progSeisGth->uniformLocation("ColorMap");
        //Q_ASSERT(m_uTexColrMapLocGthProg != -1);

        //m_v3MnMxRtLocGthProg = m_progSeisGth->uniformLocation("ampMnMxRtMxIndx");
        //Q_ASSERT(m_v3MnMxRtLocGthProg != -1);

        return false;
    }

    bool cGLWindow::VAO_VisualRef(GLuint& VAO, GLint SymbLoc)
    {
//        GLuint VBO, EBO;

//        glGenVertexArrays(1, &VAO);
//        Q_ASSERT(VAO != 0); //VAO should not be equal to Zero

//        glBindVertexArray(VAO);

//        glGenBuffers(1, &VBO);
//        glGenBuffers(1, &EBO);

//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(m_HoriTrcTrck), m_HoriTrcTrck, GL_STATIC_DRAW);

//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_2dTrckCompInd), m_2dTrckCompInd, GL_STATIC_DRAW);

//        //Update and Enable vertex position attribute in the shader,
//        //Each position has 3 floats values
//        //Stride between each location points is 5
//        glVertexAttribPointer(SymbLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//        glEnableVertexAttribArray(SymbLoc);

        return false;
    }

    /// <summary>
    /// NORMALIZING the East, North and Elevation(-Depth) values to -1 to +1 range.
    /// Sets Normalization factors for spatial and vertical coordinates
    /// </summary>
    /// <returns></returns>
    bool cGLWindow::InitCoordNormMat()
    {
        // Creating scalar for Normalizing the coordinates
        GLfloat sclCrd = 1 / ((m_maxEasting > m_maxNorthing) ? (m_maxEasting) : (m_maxNorthing));

        if (!qIsFinite(sclCrd))
        {
            cErrWarnInfo::EWI(ewiERROR, "Problem computing the coordinate min, max or normalizing factors for the data.");
            return false;
        }

        /// Elevation (-Depth) can have a different scaling factor from the spatial scaling factor.
        /// User can set the m_sclDpth to insrease or decrease the elevation scaling. A value of m_sclDpth
        /// means that elevation scaling is same as spatial scaling.
      
        GLfloat sclDpth = sclCrd * m_sclDpth;
        m_v3CrdScale = QVector3D(sclCrd, sclDpth, sclCrd);

        return false;
    }

    bool cGLWindow::InitProjViewMatrices(std::shared_ptr<QOpenGLShaderProgram> prog)
    {
        if (!prog->isLinked())
            return false;

        GLint uProjLoc = prog->uniformLocation("uProj");
        Q_ASSERT(uProjLoc != -1);

        GLint uViewLoc = prog->uniformLocation("uView");
        Q_ASSERT(uViewLoc != -1);

        /// PROJECTION. Setting projection to Prespective or Orthographic 

            /// Reset Matrix
        m_projection.setToIdentity();

        if (m_isProjPresp)
            m_projection.perspective(camera->Zoom, m_aspect, 0.1f, 4.0f); // Set perspective projection
        else
            m_projection.ortho(m_minEasting, m_maxEasting, m_minElevation, m_maxElevation, m_minNorthing, m_maxNorthing);//Set projection to Orthographic

        prog->setUniformValue(uProjLoc, m_projection);

        /// END: PROJECTION. Setting projection to Prespective or Orthographic      


        /// VIEW Matrix, taken from the camera class
        m_matView.setToIdentity();
        m_matView = camera->GetViewMatrix();
        m_matView.rotate(-m_rotationAngel, m_rotationAxis); //-m_rotationAngel has a negative sign so that 3D view rotates in same direction as the mouse movement
        prog->setUniformValue(uViewLoc, m_matView);
        /// END: VIEW Matrix

        return true;
    }

    //All Header objects stored in this list will be displayed in the 3D window
    bool cGLWindow::AddSeisHdrObj(std::shared_ptr<cSeisData> sDt, std::shared_ptr<cSeisHdr> hdr)
    {
        m_LstSeisHdrObjs.push_back(hdr);

        /// storing the current iterator position. This will be used to the remove the entry in
        /// the cGLWindow::RemoveSeisHdr(std::shared_ptr<cSeisHdr> hdr)
        hdr->m_posItr = m_LstSeisHdrObjs.end() - 1;

        ///  Everytime a 3D object is added or removed the Min and Max dimensions migth change,
        ///  so these three functions should be called in this order.
              
        SetGlobalMinMax(sDt);
        SetGlobalAvrgLoc(sDt);

        InitCoordNormMat();

        VAO_SeisHdr(hdr);

        update();

        return false;
    }

    /// <summary>
    /// Creating GL buffers and uploading data to them.
    /// This function updates the m_VAO object of the cSeisHdr. 
    /// </summary>
    /// <param name="hdr"></param>
    /// <returns></returns>
    bool cGLWindow::VAO_SeisHdr(std::shared_ptr<cSeisHdr> hdr)
    {
        try
        {
            glGenVertexArrays(1, &hdr->m_VAO);
            Q_ASSERT(hdr->m_VAO != 0); //VAO should not be equal to Zero

            glBindVertexArray(hdr->m_VAO); //bind a vertex array object

            glGenBuffers(1, &hdr->m_SymbVerBuf); // generate buffer for storing the header symbol vertices
            Q_ASSERT(hdr->m_SymbVerBuf != 0);

            glBindBuffer(GL_ARRAY_BUFFER, hdr->m_SymbVerBuf);
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_Cube), m_Cube, GL_STATIC_DRAW);

            glVertexAttribPointer(m_aSymbVertLocHdrProg, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0); //define an array of generic vertex attribute data
            glEnableVertexAttribArray(m_aSymbVertLocHdrProg); //Enable vertex attribute array

            //glBindVertexArray(0);

            return true;
        }

        catch (exception e)
        {
            return false;
        }
    }

    //All Seismic gathers stored in this list will be displayed in the 3D window
    bool cGLWindow::AddSeisGath(std::shared_ptr<cSeisGather> gth)
    {
        if (!gth->m_isGthrDisplayed)
        {
            m_LstSeisXZYGths.push_back(gth);
            gth->m_isGthrDisplayed = true;

            /// storing the current iterator position. This will be used to the remove the entry in
            /// the cGLWindow::RemoveSeisGath(std::shared_ptr<cSeisGather> gth)
            gth->m_posItr = m_LstSeisXZYGths.end() - 1;

            gth->m_glTextureNum = GetAvailTextureNum(); // Assigning the Unique texture number
            if (gth->m_glTextureNum == -1)
                return false;

            ///  Everytime a 3D object is added or removed the Min and Max dimensions migth change,
            ///  so these two functions should be called.
            SetGlobalMinMax(gth->m_ptrParent);
            SetGlobalAvrgLoc(gth->m_ptrParent);
            InitCoordNormMat();

            //// Upload seismic samples to GPU as GL_TEXTURE_2D_ARRAY texture
            Load2DSeisTrcTexture(gth);

            VAO_2dRectangle(gth->m_VAO, gth->m_isTrckVert); //Creates the tracks to display the seismic samples
        }

        update();

        return false;
    }

    /// Function checks the m_glTextureNum attribute for all the currently existing gahter objects
    /// in the m_LstSeisXZYGths and returns a number that is currently unused
    int cGLWindow::GetAvailTextureNum()
    {
        /// Total number of textures that can be simultaneously displayed
        /// I do not expect that this number of textures would ever be reached
        int MAX_TEXTURE_OBJECTS = 200;  
        int i = 0;

        std::vector<uint> inUseNums;
        inUseNums.reserve(m_LstSeisXZYGths.size());

        for (auto gth : m_LstSeisXZYGths)
        {
            inUseNums.push_back(gth->m_glTextureNum);
        }


        for (; i < MAX_TEXTURE_OBJECTS; ++i) 
        {
            std::vector<uint>::iterator it =
            
                std::find(inUseNums.begin(), inUseNums.end(), i);
            
            if (it == inUseNums.end())
                return i;

            if (i > ( MAX_TEXTURE_OBJECTS - 2 ))
            {
                cErrWarnInfo::EWI(ewiERROR, "Maximum number of allowed Seismic gather objects reached.");
                return -1;
            }
        }

        return -1;

    }

    //Removes the Seismic Gather from the 3D display, deletes the any Texture loaded to the GPU
    bool cGLWindow::RemoveSeisGath(std::shared_ptr<cSeisGather> gth)
    {
        glDeleteTextures(1, &gth->m_glTextureID);
        m_LstSeisXZYGths.erase(gth->m_posItr);
        gth->m_isGthrDisplayed = false;

        update();

        return false;
    }

    bool cGLWindow::RemoveSeisHdr(std::shared_ptr<cSeisHdr> hdr)
    {
        glDeleteBuffers(1, &hdr->m_SymbVerBuf);

        glDeleteVertexArrays(1, &hdr->m_VAO);

        m_LstSeisHdrObjs.erase(hdr->m_posItr);

        update();

        return false;
    }
    
   /// <summary>
    /// Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been
    /// resized (and also when it is shown for the first time because all newly created widgets
    /// get a resize event automatically).
    /// </summary>
    /// <param name="w">Resized window width.</param>
    /// <param name="h">Resized window height.</param>
    void cGLWindow::resizeGL(int w, int h)
    {
        // Calculate aspect ratio
        m_aspect = qreal(w) / qreal(h ? h : 1);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update(); // request to update the 3D window;

    }

    /// <summary>
    /// Renders the OpenGL scene. Gets called whenever the window needs to be updated.
    /// </summary>
    void cGLWindow::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Render();
    }

    void cGLWindow::Render()
    {

       if (!m_progHeader->bind())
            return;

       if(!InitProjViewMatrices(m_progHeader))
           return;

       //TODO add code to display compass and blank cube here ////
       // Rendering Visual Reference uses the m_proHeader shader
       // So this shader program will be connected first.
       RenderVisRef(m_progHeader);

              
        // Only call this renderer functions if there are objects to render
        if (m_LstSeisHdrObjs.size() > 0 )
        { 
            RenderSeisHdrs(m_progHeader);
        }
        
        m_progHeader->release();
        

        // Only call this renderer functions if there are objects to render
        if (m_LstSeisXZYGths.size() > 0) 
        {            
            if (!m_progSeisGth->bind())
                return;

            if (!InitProjViewMatrices(m_progSeisGth))
                return;

            //RenderSeisGath(m_progSeisGth, m_matCrdNorm);
            EXP_RenderSeisGath(m_progSeisGth);

            m_progSeisGth->release();
        }
                
    }

    /// <summary>
    /// Function renders the Grids, Compass, any Title texts. 
    /// </summary>
    /// <param name="program"></param>
    /// <param name="matCrdNorm"></param>
    /// <returns></returns>
    bool cGLWindow::RenderVisRef(std::shared_ptr<QOpenGLShaderProgram> program)
    {

        if (!m_VaoRefGrid)
            return false;

        glBindVertexArray(m_VaoRefGrid);

        QMatrix4x4 matModel; //Constructs an Identity Matrix

        QVector3D CompassLoc = QVector3D(-0.3f, -0.3f, -0.3f); //QVector3D(-0.5f, -0.5f, -0.5f);

        matModel.setToIdentity();

        matModel.translate(CompassLoc);

        //Red Axis Arm
        matModel.rotate(90.0f, 1.0f, 0.0f, 0.0f);
        program->setUniformValue(m_uModelLocHdrProg, matModel);
        program->setAttributeValue(m_aSymbScalLocHdrProg, QVector3D(1.0f, 0.001f, 0.0f));
        program->setAttributeValue(m_aColorLocHdrProg, QVector3D(1.0f, 0.0f, 0.0f));
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);

        //Green Axis Arm
        matModel.rotate(90.0f, 1.0f, 0.0f, 0.0f);
        program->setUniformValue(m_uModelLocHdrProg, matModel);
        program->setAttributeValue(m_aSymbScalLocHdrProg, QVector3D(0.001f, 1.0f, 0.0f));
        program->setAttributeValue(m_aColorLocHdrProg, QVector3D(0.0f, 1.0f, 0.0f));
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);

        //Blue Axis Arm
        matModel.rotate(90.0f, 0.0f, 1.0f, 0.0f);
        program->setUniformValue(m_uModelLocHdrProg, matModel);
        program->setAttributeValue(m_aSymbScalLocHdrProg, QVector3D(1.0f, 0.001f, 0.0f));
        program->setAttributeValue(m_aColorLocHdrProg, QVector3D(0.0f, 0.0f, 1.0f));
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        return true;
    }

    //This function will render all headers stored in the m_LstSeisHdrXZYObjs, to the 3D window
    void cGLWindow::RenderSeisHdrs(std::shared_ptr<QOpenGLShaderProgram> program)
    {

        //Cycle through all header objects
        for (auto nObj : m_LstSeisHdrObjs)
        {
            glBindVertexArray(nObj->m_VAO); //bind a vertex array object

            QVector3D v3CrdScl = m_v3CrdScale;
            if (nObj->m_flipElev)  /// In case the SegY headers have Depth instead of Elevation, user can flip the vertical axis
                v3CrdScl.setY(-m_v3CrdScale.y());

            /// Model matrix is updated for every receiver, updated matrix is further used to create copies of the 
            /// track (m_2dTrckVert) at each receiver location
            QMatrix4x4 matModel; //Constructs an Identity Matrix

            QVector3D NormCoord;

            for (uint i = 0; i < (uint)nObj->m_VcLocXYZ->size(); ++i)
            {
                matModel.setToIdentity();

                NormCoord = QVector3D(nObj->m_VcLocXYZ->at(i).xLoc, //X value
                                        nObj->m_VcLocXYZ->at(i).zLoc, //Z value
                                        nObj->m_VcLocXYZ->at(i).yLoc) // Y value
                                        - m_v3GlobalAvgEstElvNrth; // Center location for all displayed objects

                NormCoord = NormCoord * v3CrdScl;

                matModel.translate(NormCoord); // Coordinates Normalization Matrix

                program->setUniformValue(m_uModelLocHdrProg, matModel);

                //ToDo : Link symbol size with header values.
                /// <summary>
                ///  Symbol scaling is put inside the loop, because in future symbol sizes will be linked with header values
                /// </summary>
                program->setAttributeValue(m_aSymbScalLocHdrProg, QVector3D(0.003f, 0.003f, 0.003f));

                //ToDo : Link symbol color with header values.
                /// <summary>
                ///  Symbol Color is updated inside the loop, because in future symbol colors will be linked with header values
                /// </summary>
                program->setAttributeValue(m_aColorLocHdrProg, QVector4D(nObj->m_Color.redF(), nObj->m_Color.greenF(), nObj->m_Color.blueF(), nObj->m_Color.alphaF()));


                //ToDo: Use vertices and indices and glDrawElements() to reduce the number of vertices passed to GPU 
                glDrawArrays(GL_TRIANGLES, 0, 36); //Drawing the symbols

                program->setAttributeValue(m_aColorLocHdrProg, QVector4D(0.0, 0.0, 0.0, 1.0)); //Drawing the black outlines of the symbols
                glDrawArrays(GL_LINE_LOOP, 0, 36);

            }

            glBindVertexArray(0);

        }//for (auto hObj : m_LstSeisHdrXZYObjs)
 
    }

    //This function will render all Seismic gathers stored in the m_LstSeisXZYGths, to the 3D window
    bool cGLWindow::EXP_RenderSeisGath(std::shared_ptr<QOpenGLShaderProgram> program)
    {
        for (auto gth : m_LstSeisXZYGths) //List of all gathers that will be displayed
        {
            if(gth->m_glTextureID) // Texture ID should already be set to proceed
            {
                glBindVertexArray(gth->m_VAO);
                program->setUniformValue(m_uTexSamLocGthProg, gth->m_glTextureNum);

                QVector3D v3CrdScl = m_v3CrdScale;
                if (gth->m_GatherAttributes->m_flipElev)  /// In case the SegY headers have Depth instead of Elevation, user can flip the vertical axis
                    v3CrdScl.setY(-m_v3CrdScale.y());

                program->setAttributeValue(m_aSymbScalLocGthProg, gth->m_smplsTrckScal); //ToDo: This vertical scaling should match the scaling factor used for the header objects rendering

                /// Model matrix is updated for every receiver, updated matrix is further used to create copies of the 
                /// track (m_2dTrckVert) at each receiver location
                QMatrix4x4 matModel; //Constructs an Identity Matrix

                QVector3D NormCoord;
                
                QString hEast = gth->m_GatherAttributes->m_HdrNames.at(0);
                QString hNorth = gth->m_GatherAttributes->m_HdrNames.at(1);
                QString hElev = gth->m_GatherAttributes->m_HdrNames.at(2);

                qreal dEast = 0.0;
                qreal dNorth = 0.0;
                qreal dElev = 0.0;

                uint trc = 0;

                for (int i = 0; i < gth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces.size(); ++i)
                {
                    matModel.setToIdentity();

                    trc = gth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces[i];

                    dEast = gth->m_ptrParent->GetHdrVal(hEast, trc).toDouble();
                    dNorth = gth->m_ptrParent->GetHdrVal(hNorth, trc).toDouble();
                    dElev = gth->m_ptrParent->GetHdrVal(hElev, trc).toDouble();

                    NormCoord = QVector3D(dEast, dElev, dNorth);
                    NormCoord -= m_v3GlobalAvgEstElvNrth;
                    NormCoord *= v3CrdScl;

                    matModel.translate(NormCoord);

                    program->setUniformValue(m_uModelLocGthProg, matModel);
                    program->setUniformValue(m_uLayerLocGthProg, trc);

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                }

                glBindVertexArray(0);

            }

            else
            {
                cErrWarnInfo::EWI(ewiERROR, "Problem loading samples to the GPU for the current seismic gather.");
                return false;
            }


        }//for (auto hObj : m_LstSeisXZYGths)

        return true;

    }

    /// <summary>
    /// Function creates a VAO for a 2D rectangle.
    /// 2D rectangles are used in displaying Seismic waveforms as textures and
    /// Visual reference horizontal and vertical background grids
    /// </summary>
    /// <param name="VAO"></param>
    /// <returns></returns>
    bool cGLWindow::VAO_2dRectangle(GLuint& VAO, bool isTrckVertical)
    {
        GLuint VBO, EBO;

        glGenVertexArrays(1, &VAO);
        Q_ASSERT(VAO != 0); //VAO should not be equal to Zero

        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        if (isTrckVertical) //For surface seismic data the tracks are drawn verticaly, at the receiver / source location
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(m_VertTrcTrck), m_VertTrcTrck, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_TrcTrck_Indices), m_TrcTrck_Indices, GL_STATIC_DRAW);
        }
        
        //Update and Enable vertex position attribute in the shader,
        //Each position has 3 floats values
        //Stride between each location points is 5
        if (m_aSymbVertLocGthProg >= 0)   //Only for Debug : in case GLSL compiler has optimized out the attribute
        {
            glVertexAttribPointer(m_aSymbVertLocGthProg, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(m_aSymbVertLocGthProg);
        }

        if (m_aTexCoordLocGthProg >=0)  //Only for Debug : in case GLSL compiler has optimized out the attribute
        {
            glVertexAttribPointer(m_aTexCoordLocGthProg, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(m_aTexCoordLocGthProg);
        }

        ///// https://stackoverflow.com/questions/25412767/opengl-glbindbuffer0-outside-vao
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        return false;
    }
    
    /// <summary>
    /// This function uploads seismic trace samples to the GPU as 2D array (GL_TEXTURE_2D_ARRAY) of textures.
    /// If successful it returns True and updates the value of trcTex to the opengl ID of the loaded data.
    /// </summary>
    /// <param name="gth"> is the shared pointer of class "cSeisGather" having trace samples to be uploaded to the GPU.</param>
    /// <returns>   True if successful, Flase otherwise.</returns>
    bool cGLWindow::Load2DSeisTrcTexture(std::shared_ptr<cSeisGather> gth)
    {
        glGenTextures(1, &gth->m_glTextureID);
        glActiveTexture(GL_TEXTURE0 + gth->m_glTextureNum);// m_uTexSamLocGthProg);
        glBindTexture(GL_TEXTURE_2D_ARRAY, gth->m_glTextureID);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // set texture filtering parameters
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);

        const uint nSamp = gth->m_ptrParent->m_NumSamples; // Seismic samples are placed from start to end of the trace
        const uint HEIGHT = 1; 
        const uint nTraces = gth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces.size(); // The number of traces in the GATHER       
        const uint OFFSET = 0;

        //GLR32F does not have alpha channel
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, nSamp, HEIGHT, nTraces);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, OFFSET, OFFSET, OFFSET, nSamp, HEIGHT, nTraces, GL_RGBA, GL_UNSIGNED_BYTE, gth->m_SamplesRGB->data());
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        return true;
    }

    void cGLWindow::mousePressEvent(QMouseEvent* ev)
    {
        m_mousePressPos = QVector2D(ev->localPos());
    }

    void cGLWindow::mouseMoveEvent(QMouseEvent* ev)
    {

        //PAN 3D View
        if (ev->buttons() == Qt::RightButton)
        {

            float xOffset = m_mousePressPos.x() - ev->localPos().x();
            float yOffset = ev->localPos().y() - m_mousePressPos.y();

            m_mousePressPos = QVector2D(ev->localPos());

            camera->ProcessMouseMovement(xOffset, yOffset);

            update();
        }

        //ROTATE 3D view
        if (ev->buttons() == Qt::LeftButton)
        {
                // Mouse release position - mouse press position
                QVector2D diff = QVector2D(ev->localPos()) - m_mousePressPos;

                // Rotation axis is perpendicular to the mouse position difference
                // vector
                QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

                // Accelerate angular speed relative to the length of the mouse sweep
                qreal acc = diff.length() / 500.0;

                // m_rotationAngel = m_rotationAngel - acc; //Used hit and trial to come up with this relation. ToDo - See if you can refine it

                // Calculate new rotation axis as weighted sum
                //m_angularSpeed = 1;
                //m_rotationAxis = (m_rotationAxis * m_angularSpeed + n * acc).normalized();
                m_rotationAxis = (m_rotationAxis + n * acc).normalized();

               update();
        }
    }

    /// <summary>
    /// 3D window's ZOOM functionlaity is implemented here
    /// </summary>
    /// <param name="event"></param>
    void cGLWindow::wheelEvent(QWheelEvent* event)
    {
        //Code snippet taken from https://doc.qt.io/qt-5/qwheelevent.html

        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta() / 8; //Increasing the denominator, will decrease the zooming steps

        if (!numPixels.isNull()) {
            camera->ProcessMouseScroll(numPixels.y() * ZOOM_SPEED);
        }
        else if (!numDegrees.isNull()) {
            QPoint numSteps = numDegrees / 15;
            camera->ProcessMouseScroll(numSteps.y() * ZOOM_SPEED);
        }

        update();
    }

    void cGLWindow::printContextInformation()
    {
        QString glType;
        QString glVersion;
        QString glProfile;

        // Get Version Information
        glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
        glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
        switch (format().profile())
        {
            CASE(NoProfile);
            CASE(CoreProfile);
            CASE(CompatibilityProfile);
        }
#undef CASE

        // qPrintable() will print our QString w/o quotes around it.
        qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
    }

    void cGLWindow::SetGlobalMinMax(std::shared_ptr<cSeisData> sDt)
    {
        Update3DWindowMinEasting(sDt->GetMinX());
        Update3DWindowMaxEasting(sDt->GetMaxX());
        Update3DWindowMinElevation(sDt->GetMinZ());
        Update3DWindowMaxElevation(sDt->GetMaxZ());
        Update3DWindowMinNorthing(sDt->GetMinY());
        Update3DWindowMaxNorthing(sDt->GetMaxY());

    }

    /// <summary>
    /// Sets m_v3GlobalAvrgLoc.
    /// Should be called everytime a new m_LstSeisHdrObjs or m_LstSeisXZYGths is added 
    /// or removed.
    /// </summary>
    void cGLWindow::SetGlobalAvrgLoc(std::shared_ptr<cSeisData> sDt)
    {
        QVector3D avg = (sDt->GetRcvLocAvg() + sDt->GetSrcLocAvg()) / 2;
        
        // Flipping the z and y locations to stay consistent my OpenGL convention. As opengl Y is the vertical and Z is the depth axis
        m_v3GlobalAvgEstElvNrth = QVector3D(avg.x(), avg.z(), avg.y()); 

    }

//}




