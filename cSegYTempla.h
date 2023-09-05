#pragma once
#include <QString>
#include <QVector>

//namespace GnV //GnGViewer
//{
    class cSegYTempla
    {

    public:
        bool         m_IsTrcHdr;
        bool         m_IsDblHdr; //Is Double Header
        QString      m_HdrName;
        QString      m_HdrDesc;
        bool         m_HdrLoad;
        bool         m_isLngtTwoByt = false; //Only dealing with 2 or 4 byte lengths
        quint32      m_HdrIndex;     //Header's row number in the SegY template file and TableWidget.
        QString      m_HdrType;      // Is 2 byte or 4 Byte   
        quint32      m_StrtByte;     //Header's byte location within the 240bytes trace headers
        quint32      m_minValue;
        quint32      m_maxValue;

    };

//}
