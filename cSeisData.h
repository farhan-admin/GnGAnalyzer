#ifndef CSeisData_H
#define CSeisData_H

#include <memory>
#include <QAbstractTableModel>
#include <QColor>
#include <QVector>
#include <QVector3D>

#include "cSegYTempla.h"
#include "cSeisHdr.h"
#include "cSeisGather.h"

#include "cbinhdr.h"
#include "csortorder.h"
#include "cerrwarninfo.h"
#include "AllEnums.h"

//namespace GnV //GnGViewer
//{

    // Class to store meta data for the seismic traces
    // This is the Model class in the Qt Model View design
    class cSeisData : public QAbstractTableModel
    {
        Q_OBJECT

    public:
        ~cSeisData();

        cSeisData(QObject* parent = nullptr);

        bool                    m_IsBinHdrAvail;    // Flag to see if Binary header specifies the number of traces
        QVector<CBinHdr>        m_BinHdrs;          // Binary headers for the SegY file
        QVector<CSortOrder>     m_SortOrders;       // The QVector to store sorting orders created for this data set
        QVector<cSegYTempla>    m_SegYTemplate;     // SegY Template parameters for this data
        QString                 m_DataName;         // Name of the Data file
        QVector<QString>        m_Desc;             // Short history of the data
        QString                 m_SgyFullFilePath;  // Full SegY file path
        QString                 m_SgyTmplFilePath;  // Full file path of the SegY template file
        quint64                 m_NumOfPredicTrc;   // PREDICTED number of traces in the SEG data file, initially this is computed using file and header size, to reserve the memory.
        quint64                 m_TrcCounter;       // This counter keeps a track of how many traces were actually read.
        QVector<QString>        m_EbcdHdr;          // List to store original or edited EBCDIC or ASCII header
        quint64                 m_TraceStride;      // Trace Byte length (including trace headers and samples)
        quint64                 m_NumOfTrcs;        // LOADED number of traces by the user
        quint64                 m_NumOfHdrs;        // Number of headers loaded for this data
        quint64                 m_NumSamples;       // Number of samples in the traces, this is read from the Binary Header
        DataType                m_dataType;         // ZVSP, WVSP, Migrated
        
        /// Pointers to all the gather objects
        QVector< std::shared_ptr<cSeisGather> > m_ptrChildGather;

        /// Pointers to all the header objects
        QVector< std::shared_ptr<cSeisHdr> > m_ptrChildHdr;

        std::shared_ptr<cColorMap> m_ColorMap;
        
        void InitSeisTrc(quint64 nTracs);
        void Clear(); //Removes data from m_SeisTrcHdrs and m_TrcHdrNames

        void AddBinHdr(QString inName, QString inDesc, long inVal);
        std::shared_ptr<CBinHdr> GetBinHdr(QString inhdrName);
        quint32 GetNumOfBinHdrs() { return (quint32)m_BinHdrs.size(); }
        bool SetEBCDIChdr(char* bufEbcdic); //Set the EBCDIC header, stores text in stringList for readability, returns number of lines
        std::shared_ptr<QVector<QString> > GetEBCDIChdr(); //Return EBCDIC header in List form
        quint64 GetCurrentTrcNum() { return m_TrcCounter; }
        quint64 IncrementTrcCount() { ++m_TrcCounter; return m_TrcCounter; }
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        bool SetHdrVal(QString name, quint64 TrcNum, double val);
        bool TmplteIsDblTrcHdr(QString name) const;
        QVariant GetHdrVal(QString name, quint64 TrcNum) const;
        QVariant GetHdrVal(qint64 hdrNum, quint64 trcNum) const;
        void Multiply(QString Header, qint64 scalar);
        void Divide(QString Header, qint64 divisor);
        bool GetSameHdrForAllTrcs(QString name, qint64& strtIndx, quint64& lngth);
        //std::shared_ptr< QVector<double> > GetHeadersForGL(QString hEast, QString hNorth, QString hElev, QVector<QString> hAttrib, quint64 FrmTrc, quint64 ToTrc);
        
        /// <summary>
        /// Compares the provided headers to identify duplicate values.
        /// Each duplicate value is returned only once.
        /// </summary>
        /// <param name="xHdr">First header to be used in comparison</param>
        /// <param name="yHdr">Second header to be used in comparison</param>
        /// <param name="zHdr">Third header to be used in comparison</param>
        /// <param name="vLoc">This function updates the vLoc vector by storing the found unique locations.</param>
        /// <returns>see vLoc description</returns>
        void FindUniqueLoc(QString xHdr, QString yHdr, QString zHdr, QVector<cLocXYZ>& vLoc);
        void GetLocHdrs(QString xHdr, QString yHdr, QString zHdr, QVector<cLocXYZ>& vLoc);
        bool GetTraceGather(bool isSrcGather, quint32 GathNum, std::shared_ptr <cSeisGather> seisGth);
        bool GetStackedData(std::shared_ptr <cSeisGather> seisGth, QString& xHdr, QString& yHdr, QString& zHdr, quint64& frmTrc, quint64& toTrc);
        std::shared_ptr<QVector<cLocXYZ>> GetSrcRcvs(bool getSrc);
        std::shared_ptr<QVector<QVariant>> GetAllHdrsForATrc(quint64 TrcNum); //Returns pointer vector with all headers for a trace.

         /// <summary>
         /// Calling these function with a new low/high value, sets the m_GlobalMin/m_GlobalMax to val.
         /// If called without the "val" parameter than function will return the current min/max value.
         /// </summary>
         /// <param name="val">New value to set.</param>
         /// <returns></returns>
        void inline SetMinX(qreal val)
        {
            if (val < m_minX)
                m_minX = val;
        };
        void inline SetMaxX(qreal val)
        {
            if (val > m_maxX)
                m_maxX = val;
        };
        void inline SetMinY(qreal val)
        {
            if (val < m_minY)
                m_minY = val;
        };
        void inline SetMaxY(qreal val)
        {
            if (val > m_maxY)
                m_maxY = val;
        };
        void inline SetMinZ(qreal val)
        {
            if (val < m_minZ)
                m_minZ = val;
        };
        void inline SetMaxZ(qreal val)
        { 
            if (val > m_maxZ)
                m_maxZ = val;
        };
        qreal inline GetMinX() { return m_minX; };
        qreal inline GetMaxX() { return m_maxX; };
        qreal inline GetMinY() { return m_minY; };
        qreal inline GetMaxY() { return m_maxY; };
        qreal inline GetMinZ() { return m_minZ; };
        qreal inline GetMaxZ() { return m_maxZ; };

        void inline SetSrcLocAvg(QVector<cLocXYZ>& vLocs) { m_srcLocAvg = ComputeLocAvrg(vLocs); };
        void inline SetRcvLocAvg(QVector<cLocXYZ>& vLocs) { m_rcvLocAvg = ComputeLocAvrg(vLocs); };
        QVector3D inline GetSrcLocAvg() { return m_srcLocAvg; };
        QVector3D inline GetRcvLocAvg() { return m_rcvLocAvg; };


        void UpdateMinMax();
        QVector3D ComputeLocAvrg(QVector<cLocXYZ>& vLocs);
        QVector3D ComputeLocAvrg(QVector3D& pt1, QVector3D& pt2);

        QVector<cLocXYZ>        m_UniqSrcs;     // Vector storing all unique sources and numbers of their associated traces
        QVector<cLocXYZ>        m_UniqRcvs;     // Vector storing all unique receivers and numbers of their associated traces  

        // Headers are stored in the same sequence they appear in the SegYTemplate
        QVector<QVariant>		m_SeisTrcHdrs;	//Array of double type storing all double headers in a contigent memory block
        QVector<QString>        m_TrcHdrNames;     // Header names. Index of the header gives the index of the trace values in the
                                                  // m_SeisTracHdrs array.
                                                  // Names should be stored in the same order as in the cSegyTemplate
                                                  // Byte location of the trace from start of the SegY file
        /*
        In a row major matrix, elements are stored so that row index changes slowest (r,c), 
        where "r" is hdrIndx that is header number in the list and "c" is the trace number

            1 r =  1 x nCols
            N r =  N x nCols

        so location (r,c) = nCols + c;

        In our case
            r = Header index
            c = Trace number
        So going along a row and across all columns, you will find a common header for all traces.
        And going along a colum and across all rows, you will find all headers for a common trace.
        
        Header Layout (where "T1" is "trace # 1", "H1" is "header # 1")
        Column->    1         2          3     ...     n
        Row        T1H1      T2H1       T3H1          TnH1
        |          T1H2      T2H2       T3H2          TnH2
        v          T1H3      T2H3       T3H3          TnH3
                   .
                   .
                   .
                   T1Hn      T2Hn       T3Hn          TnHn

    */
        quint64 inline ConvertIndxMatToRow(quint64 hdrIndx, quint64 trcNum) const
        {
            return (hdrIndx * m_NumOfTrcs + trcNum);
        }

        /// <summary>
        /// If data header has flipped sign for elevation (depth) headers
        /// user can set this parameter to flip the sign.
        /// </summary>
        bool m_flipElev;

        /// <summary>
        /// The amount of display normalization applied to the 
        /// seismic waveforms
        /// </summary>
        float m_dispNorm;


    private:
        qreal m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
        QVector3D m_srcLocAvg, m_rcvLocAvg;
        

    };
//}
#endif // CSeisData_H
