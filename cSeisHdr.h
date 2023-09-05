#pragma once
#include <memory>
#include <QVector>
#include <QColor>
#include <QLinkedList>
#include "cLocXYZ.h"
#include "cColorMap.h"

/// <summary>
/// Class to pass seismic HEADERS to the OpenGL window for rendering
/// The format is locationA,header1,header2,...,headerN,locationB,header1,header2,...,headerN,...
/// Location is stored a X = East, Z = North and Y = Elevation to be consistent with OpenGL axes convention
/// Y-axis in OpenGL is vertical, X and Z are lateral axes.
/// Number of headers is specified in parameter "m_NumHdrs". The number of headers could be from ZERO to N.
/// ZERO headers means that only locations are passed, e.g. Source, Receiver locations
/// Names of the headers are stored in the "m_HdrNames" parameter.
/// </summary>
class cSeisHdr
{
public:
    cSeisHdr(quint64 nTraces = 0, quint32 nHdrs = 0) : m_NumHdrs(nHdrs), m_NumOfTraces(nTraces), m_VAO(0), m_flipElev(false), m_SymbVerBuf(0)
    {}
    std::shared_ptr< QVector<cLocXYZ>>  m_VcLocXYZ = std::make_shared< QVector<cLocXYZ>>();
    std::shared_ptr<cLocXYZ> m_ptrGatherLocAndAssoTrc = std::make_shared<cLocXYZ>();

    quint16 m_NumHdrs; //This can be zero, in case only receivers and sources are displayed
    quint64 m_NumOfTraces;
    uint m_VAO; // VAO should be updated before rendering in function cGLWindow::SetSeisHdrGLBuffers(std::shared_ptr<cSeisHdr> hdr)
    QColor m_Color; //A single solid color for displaying the object.
    QVector<QString> m_HdrNames; // Header names


    //std::shared_ptr<cColorMap> m_ColorMap;// = std::make_shared<cColorMap>(); //ColorMap is used for coloring objects using header values or color samples values in the traces

    /// <summary>
    /// If data header has flipped sign for elevation (depth) headers
    /// user can set this parameter to flip the sign.
    /// </summary>
    bool m_flipElev;

    /// <summary>
    /// Stores the reference to the buffer on the GPU where vertex objects are stored.
    /// </summary>
    uint m_SymbVerBuf;

    /// <summary>
    /// The iterator is used when deleting the pointer, in the function cGLWindow::RemoveSeisGath(std::shared_ptr<cSeisGather> gth)
    /// </summary>
    QList<std::shared_ptr<cSeisHdr>>::iterator m_posItr;
};

