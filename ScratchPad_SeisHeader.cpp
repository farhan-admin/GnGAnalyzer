void cGLWindow::RenderSeisHdrs(QOpenGLShaderProgram* program, QMatrix4x4 matCrdScl)
{
    //Cycle through all header objects
    quint32 nObjs = m_LstSeisHdrObjs.size();

    if (nObjs == 0)
        return; // No need to continue if there are no objects to render

    GLuint VAO, SymbVerBuf, EBO;
    glGenVertexArrays(1, &VAO); //generate vertex array object names

    QMatrix4x4 matModel; //Constructs an Identity Matrix

    glBindVertexArray(VAO); //bind a vertex array object

    for (auto nObj : m_LstSeisHdrObjs)
    {
        glGenBuffers(1, &SymbVerBuf); // generate buffer for storing the header symbol vertices
        
        glBindBuffer(GL_ARRAY_BUFFER, SymbVerBuf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_Cube), m_Cube, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0); //define an array of generic vertex attribute data
        glEnableVertexAttribArray(0); //Enable or disable a generic vertex attribute array
                    
        for (unsigned int i = 0; (i + 2) < nObj->m_XZYval->size(); i = i + 3)
        {
            matModel.setToIdentity();
       
            matModel.translate( QVector3D(nObj->m_XZYval->at(i),  //X value
                                          nObj->m_XZYval->at(i + 1), //Z value
                                          nObj->m_XZYval->at(i + 2) ) // Y value
                                                         * matCrdScl); // Translation Matrix

            qDebug() << "Header model" << matModel;
                
            matModel.scale(0.002, 0.001, 0.002); // Scaling the cube, X, Z, Y axis

            //float angle = 20.0f * i;
            //matModel.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));

            program->setUniformValue(m_ModelUniform, matModel);
                
            program->setUniformValue(m_ColorUniform, QVector4D( nObj->color.redF(), nObj->color.greenF(), nObj->color.blueF(), nObj->color.alphaF() ));

            glDrawArrays(GL_TRIANGLES, 0, 36); //Drawing the symbols

            program->setUniformValue(m_ColorUniform, QVector4D(0.0, 0.0, 0.0, 1.0)); //Drawing the black outlines of the symbols
            glDrawArrays(GL_LINE_LOOP, 0, 36);

        }

        glDeleteBuffers(1, &SymbVerBuf);

    }//for (auto hObj : m_LstSeisHdrXZYObjs)
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);

}