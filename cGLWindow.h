#pragma once
/*
Source code help taken from the following sources
https://www.trentreed.net/blog/qt5-opengl-part-0-creating-a-window/
https://forum.qt.io/topic/99030/rendering-in-qopenglwindow-embedded-in-qmdiarea-fails/2
*/

#include <QDebug>
#include <QOpenGLWindow>
#include <QOpenGLDebugLogger>
#include <QExposeEvent>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QtMath>
#include <QVector3D>
#include <QLinkedList>
#include <QWheelEvent>

#include "Camera.h"
#include "cSeisData.h"

class cErrWarnInfo;

//namespace GnV //GnGViewer
//{

    constexpr float ZOOM_SPEED = 3; //Sets the zoom speed in the OpenGL Window Mouse Wheel Function
                                    // The bigger the number the faster it will zoom.

    /// <summary>
    /// There are TWO compiled and linked GLSL programs.
    /// One for rendering header objects m_progHeader.
    /// Second for rendering seismic gather objects. Fragment shader in this m_progSeisGth program uses texture.
    /// Third for drawing visual refernces like grids, compass etc.
    /// </summary>
    /// <returns></returns>
    
    /// ####### For Headers and Source Receiver displays
    static const char* Shader_Vert_Headers =
        "#version 430 core\n"
        "layout (location = 0) in mediump vec3 v3inSymbVert;\n"
        "layout (location = 1) in lowp vec3 v3inSymScale;\n"
        "layout (location = 2) in lowp vec4 v4inColor;\n"
        "layout (location = 3) uniform mediump  mat4 uModel;\n" // "u" is for uniform
        "layout (location = 4) uniform mediump  mat4 uView;\n"
        "layout (location = 5) uniform mediump  mat4 uProj;\n"

        "out vec4 v4outColor;\n"

        "void main()\n"
        "{\n"
        "   vec3 v3SclSymb = v3inSymbVert * v3inSymScale;\n"
        "   gl_Position = uProj * uView * uModel * vec4( v3SclSymb, 1.0f);\n"
        "   v4outColor = v4inColor;\n"
        "}\n";


    static const char* Shader_Frag_Headers =
        "#version 430 core\n"

        "in vec4 v4outColor;\n"
        "out vec4 FragColor;\n"

        "void main()\n"
        "{\n"

        "   FragColor =  v4outColor ;\n"
         
        "}\n";
    /// ####### END For Headers and Source Receiver displays


    /// ####### SHADER For Seismic Gather displays
    static const char* Shader_Vert_SeisGth =
        "#version 430 core\n"
        "layout (location = 0) in mediump vec3 v3inSymbVert;\n"
        "layout (location = 1) in lowp vec3 v3inSymScale;\n"
        "layout (location = 2) uniform mediump  mat4 uModel;\n" // "u" is for uniform
        "layout (location = 3) uniform mediump  mat4 uView;\n"
        "layout (location = 4) uniform mediump  mat4 uProj;\n"
        "layout (location = 5) in vec2 v2inTexCoord;\n"    // "v2in" is for vec2 in

        "out vec2 v2TexCoord;\n"

        "void main()\n"
        "{\n"
        "   vec3 v3SclSymb = v3inSymbVert * v3inSymScale;\n"
        "   gl_Position = uProj * uView * uModel * vec4( v3SclSymb , 1.0f);\n"
        "   v2TexCoord = v2inTexCoord;\n"
        "}\n";


    static const char* Shader_Frag_SeisGth =
        "#version 430 core\n"

        "layout (location = 7) uniform int intLayer;\n"

        "uniform sampler2DArray SamplesAry;\n"

        "in vec2 v2TexCoord;\n"

        "out vec4 outColor;\n"

        "void main()\n"
        "{\n"
        "   outColor = texture(SamplesAry, vec3(v2TexCoord, intLayer) ) ;\n"
        "}\n";

    /// ####### END For Seismic Gather displays shaders

    
    class cGLWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions
    {
        Q_OBJECT

    public:
        explicit cGLWindow(QWindow* parent = nullptr);
        bool AddSeisHdrObj(std::shared_ptr<cSeisData> sDt, std::shared_ptr<cSeisHdr> hdr);
        bool AddSeisGath(std::shared_ptr<cSeisGather> gth);
        int GetAvailTextureNum();
        bool RemoveSeisGath(std::shared_ptr<cSeisGather> gth);
        bool RemoveSeisHdr(std::shared_ptr<cSeisHdr> hdr);
        ~cGLWindow();

    private:
        //Shape - CUBE having 36 vertices
        GLfloat m_Cube[180] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        GLfloat m_VertTrcTrck[20] =
        {
            //OpenGL Axis (Y is vertical, Z goes into the screen)          
            // X       Y      Z     Texture Coords - For VERTICAL trace display
             0.5f,   0.0f,  0.0f,   0.0f, 1.0f,   // 0 - top right
             0.5f,  -0.5f,  0.0f,   1.0f, 1.0f,   // 1 - bottom right
            -0.5f,  -0.5f,  0.0f,   1.0f, 0.0f,   // 2 - bottom left
            -0.5f,   0.0f,  0.0f,   0.0f, 0.0f    // 3 - top left 
        };

        //Indices for the 2D track that hosts samples for traces
        GLuint m_TrcTrck_Indices[6] =
        {
            0, 3, 2, // first triangle
            2, 1, 0  // second triangle
        };

    protected:
        void initializeGL() override;
        bool GLLogHandler(QOpenGLDebugMessage message);
        GLenum glCheckError_(const char* file, int line);
        bool InitShaders();
        bool UpdateShaderVarLocs();
        bool VAO_VisualRef(GLuint& VAO, GLint SymbLoc);
        bool InitCoordNormMat();
        bool InitProjViewMatrices(std::shared_ptr<QOpenGLShaderProgram> prog);
        void resizeGL(int width, int height) override;
        void paintGL() override;
        void Render();
        bool RenderVisRef(std::shared_ptr<QOpenGLShaderProgram> program);
        void RenderSeisHdrs(std::shared_ptr<QOpenGLShaderProgram> program);
        bool EXP_RenderSeisGath(std::shared_ptr<QOpenGLShaderProgram> program);
        bool VAO_2dRectangle(GLuint& VAO, bool isTrckVertical);
        bool VAO_SeisHdr(std::shared_ptr<cSeisHdr> hdr);
        bool Load2DSeisTrcTexture(std::shared_ptr<cSeisGather> gth);
        void mousePressEvent(QMouseEvent* ev) override;
        void mouseMoveEvent(QMouseEvent* ev) override;
        void wheelEvent(QWheelEvent* event) override;

    private:
        void printContextInformation();
        QOpenGLDebugLogger* m_GLlogger = new QOpenGLDebugLogger(this); //Used for Debugging OpenGL commands

        /// <summary>
        /// Locations for the Headers GLSL program
        /// </summary>
        GLint m_aSymbVertLocHdrProg = -1;
        GLint m_aSymbScalLocHdrProg = -1;
        GLint m_uModelLocHdrProg = -1; // uniform MODEL matrix location
        GLint m_aColorLocHdrProg = -1; //Color attribute location

        /// <summary>
        /// Locations for the Seismic Gather GLSL program
        /// </summary>
        GLint m_aSymbVertLocGthProg = -1;
        GLint m_aSymbScalLocGthProg = -1;
        GLint m_aTexCoordLocGthProg = -1;
        GLint m_uColrScalLocGthProg = -1;
        GLint m_uLayerLocGthProg = -1;
        GLint m_uModelLocGthProg = -1; // uniform MODEL matrix location
        GLint m_uTexSamLocGthProg = -1;
        GLint m_uTexColrMapLocGthProg = -1;
        GLint m_v3MnMxRtLocGthProg = -1;
        GLuint m_VaoRefGrid = 0; // VAO for the main reference grid


        std::shared_ptr<QOpenGLShaderProgram> m_progHeader = nullptr;
        std::shared_ptr<QOpenGLShaderProgram> m_progSeisGth = nullptr;

        int m_frame = 0;
        //bool m_redraw = true;
        GLfloat m_sclDpth = 1.0f; //if this equal to 1, coordinate scalar will be used for scaling the depth and spatial coordinates
        QVector3D m_v3CrdNorm;
        QVector3D m_v3CrdScale;
        QVector3D m_v3GlobalAvgEstElvNrth;
        QMatrix4x4 m_matView;
        QMatrix4x4 m_matCrdNorm;

        //Mouse interaction
        //Camera camera(0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f, -90.0, 0.0);
        Camera* camera = new Camera(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        bool m_mouseFirst;
        QVector2D m_mousePressPos;
        QVector3D m_rotationAxis;
        qreal m_angularSpeed = 0;
        //QQuaternion m_rotation;
        qreal m_rotationAngel;

        //Object coordinates are displayed in the X, Z and Y order to be consistent with OpenGL coordinate system
        QList< std::shared_ptr<cSeisGather> > m_LstSeisXZYGths; // List of the currently displayed seismic gathers
        QList< std::shared_ptr<cSeisHdr> > m_LstSeisHdrObjs; // List of the currently displayed seismic trace headers
        //////

        //Functions and parameters related to the 3D Graphics displays ///
    private:

        //void SetGlobalAvrgLoc();
        void SetGlobalAvrgLoc(std::shared_ptr<cSeisData> sDt);

        /// <summary>
        /// These functions control the Min and Max for the currently 
        /// visible data in the 3D window.
        /// </summary>
        /// <param name="val"></param>
        void SetGlobalMinMax(std::shared_ptr<cSeisData> sDt);
        void inline Update3DWindowMinEasting(qreal val)
        {
            if (val < m_minEasting)
                m_minEasting = val;
        }
        void inline Update3DWindowMaxEasting(qreal val)
        {
            if (val > m_maxEasting)
                m_maxEasting = val;
        }
        void inline Update3DWindowMinNorthing(qreal val)
        {
            if (val < m_minNorthing)
                m_minNorthing = val;
        }
        void inline Update3DWindowMaxNorthing(qreal val)
        {
            if (val > m_maxNorthing)
                m_maxNorthing = val;
        }
        void inline Update3DWindowMinElevation(qreal val)
        {
            if (val < m_minElevation)
                m_minElevation = val;
        }
        void inline Update3DWindowMaxElevation(qreal val)
        {
            if (val > m_maxElevation)
                m_maxElevation = val;
        }

        //Projection
        QMatrix4x4 m_projection;
        bool m_isProjPresp; //Is projection Prespective?
        qreal m_aspect;

        /*
        The OpenGL positive x-axis is towards right
        The OpenGL positive y-axis is towards up
        The OpenGL positive z-axis goes into the screen
        */
        qreal m_minEasting, m_maxEasting, m_minNorthing, m_maxNorthing, m_minElevation, m_maxElevation;

      
    };

//}
