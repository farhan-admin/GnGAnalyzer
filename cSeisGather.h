#pragma once
#include <memory>
#include <QVector>
#include <QColor>
#include <QVector3D>
#include "cLocXYZ.h"
#include "cSeisHdr.h"


class cSeisData; //Forward declaration

/// <summary>
    /// Class to store seismic trace samples. Samples are sequentially stored in a 1D array
    /// with samples for first trace followed by the next.
    /// Number of samples in each trace is same and is stored in the m_NumOfSamples parameter
    /// Number of traces in the gather is stored in the m_NumOfTraces parameter
    /// Location of traces are stored in a member of cSeisHdr type
    /// </summary>
class cSeisGather
{
public:
    cSeisGather() : m_glTextureID(0), m_glTextureNum(0), m_glColoMapTextureID(0), m_VAO(0), m_isGthrDisplayed(false), m_isSrcGather(0), m_isTrckVert(true)
    {}
    //std::shared_ptr< QVector<float> >  m_2DSamples = std::make_shared<QVector<float>>(); //Array with samples of all the traces stored sequentially
    std::shared_ptr< QVector<unsigned char> >  m_SamplesRGB = std::make_shared<QVector<unsigned char>>(); //Array with samples of all the traces stored sequentially
    std::shared_ptr<cSeisHdr>  m_GatherAttributes = std::make_shared<cSeisHdr>(); //Locations of the traces
    std::shared_ptr<cSeisData> m_ptrParent = std::make_unique<cSeisData>(); //A cSeisData object is the parent of the gather

    //qreal m_maxNegSeisAmp; //Stores this gather's maximum Negative seismic amplitude
    //qreal m_maxPosSeisAmp; //Stores this gather's maximum Positive seismic amplitude

    /// <summary>
    /// This parameter is updated in the cGLWindow::Load2DSeisTrcTex function.
    /// It stores the opengl ID for the loaded 2D texture array.
    /// m_glTextureID = 0 indicates invalid value.
    /// </summary>
    uint m_glTextureID;

    /// <summary>
    /// This stores the Texture number. It is used in the glActiveTexture functions to 
    /// activate the correct texture on while Loading texture data and rendering the texture.
    /// </summary>
    quint16 m_glTextureNum;

    /// <summary>
    /// The iterator is used when deleting the pointer, in the function cGLWindow::RemoveSeisGath(std::shared_ptr<cSeisGather> gth)
    /// </summary>
    QList<std::shared_ptr<cSeisGather>>::iterator m_posItr;

    /// <summary>
    /// This parameter is updated in the cGLWindow::LoadColorMap function.
    /// It stores the opengl ID for the loaded color map as a texture array.
    /// m_glColoMapTextureID = 0 indicates invalid value.
    /// </summary>
    uint m_glColoMapTextureID;

    /// <summary>
    /// This parameter is updated in the cGLWindow::AddSeisGath function.
    /// It stores the Vertex Array Object (VAO) for the loaded seismic gather.
    /// m_VAO = 0 indicates invalid value.
    /// </summary>
    uint m_VAO;

    /// <summary>
    /// This parameter tracks if the currect gather is already added to the m_LstSeisXZYGths. 
    /// New gathers are added to the m_LstSeisXZYGths in the cGLWindow::AddSeisGath function.
    /// This parameter stops from getting a gather added multiple times.
    /// </summary>
    bool m_isGthrDisplayed;

    /// <summary>
    /// This bool defines if the gather has single source and one or more receivers (a source gather) or 
    /// single receiver and one or more sources (a receiver gather). This is used in cGlWindow class to 
    /// decided if the trace tracks should be drawn horizontal (CSG) or vertical (CRG)
    /// </summary>
    bool m_isSrcGather;

    /// <summary>
    /// This attribute controls how the data traks will be scaled, when OpenGL draws them.
    /// </summary>
    QVector3D m_smplsTrckScal;

    /// <summary>
    /// This attribute controls, if the tracks will be drawn vertically or horizontally. TRUE = Vertical
    /// </summary>
    bool m_isTrckVert; 
};






