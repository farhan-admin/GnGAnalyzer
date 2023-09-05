#include <QFileDialog>
#include <QFont>
#include <QBrush>
#include <QString>
#include <QDebug>

#include <QMutableVectorIterator>
#include <algorithm>
#include <memory>

#include "cSegYReadWrite.h"
#include "cSeisData.h"
#include "csegyFunc.h"


//namespace GnV //GnGViewer
//{

    class cSegyReadWrite;

    cSeisData::cSeisData(QObject* parent) :
        QAbstractTableModel(parent),
        m_IsBinHdrAvail(false),
        m_DataName("Default"),
        m_SgyTmplFilePath("C:\\Users\\geofa\\source\\repos\\FarhanNaseer78\\GnGViewer_02\\SegYRev1Template_ver5.sgyt"),
        m_NumOfPredicTrc(0),
        m_TrcCounter(0),
        m_TraceStride(0),
        m_NumOfTrcs(0),
        m_NumOfHdrs(0),
        m_NumSamples(0),
        m_dataType(DataType::DownholeRcvSingleSrc),
        m_flipElev(false),
        m_dispNorm(1),
        m_minX(std::numeric_limits<double>::max()),
        m_maxX(-std::numeric_limits<double>::max()),
        m_minY(std::numeric_limits<double>::max()),
        m_maxY(-std::numeric_limits<double>::max()),
        m_minZ(std::numeric_limits<double>::max()),
        m_maxZ(-std::numeric_limits<double>::max())
    {}

    cSeisData::~cSeisData()
    {
        //Explicitly deleteing because the elements were created using "new" operator
        //and I am not sure if QVector will delete them automatically.
        if (!m_SegYTemplate.isEmpty())
        {
            m_SegYTemplate.clear();
            m_SegYTemplate.squeeze();
        }

    }

    //##############  START : Model View Setup  ###################
    int cSeisData::columnCount(const QModelIndex& /*parent*/) const
    {
        return m_NumOfTrcs;
    }

    int cSeisData::rowCount(const QModelIndex& /*parent*/) const
    {
        return m_NumOfHdrs;
    }

    QVariant cSeisData::data(const QModelIndex& index, int role) const
    {
        int row = index.row();
        int col = index.column();

        switch (role) {

        case Qt::DisplayRole:

            return GetHdrVal(row, col);

            break;

        case Qt::FontRole:
            //if (row == 0 && col == 0) { //change font only for cell(0,0)
            //    QFont boldFont;
            //    boldFont.setBold(true);
            //    return boldFont;
            //}
            break;

        case Qt::BackgroundRole:
            //return QBrush(Qt::red);
            break;

        case Qt::TextAlignmentRole: // All text is aligned center
            return Qt::AlignCenter; // +Qt::AlignVCenter;
            break;

        case Qt::CheckStateRole:
            //if (row == 1 && col == 0) //add a checkbox to cell(1,0)
            //    return Qt::Checked;
            break;

        }
        return QVariant();
    }

    QVariant cSeisData::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Vertical)
            return QString(m_TrcHdrNames[section]);

        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
            return QString("Tr# " + QString::number(section));

        if (role == Qt::FontRole)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }

        if (role == Qt::ForegroundRole)
            return QBrush(Qt::black);

        return QVariant();
    }
    //############   END : Model View Setup   #####################

    //Reserving memory for loading cSeisTrc, cSrcXYZ and cRcvXYZ objects
    void cSeisData::InitSeisTrc(quint64 nTraces)
    {
        try {

            quint16 sz = m_SegYTemplate.size();
            quint16 nHdrsToStore = 0; //Number of Trace Headers selected by the user for loading

            m_TrcHdrNames.clear(); //Removing any previously stored names
            for (quint16 i = 0; i < sz; ++i)
                if (m_SegYTemplate[i].m_HdrLoad && m_SegYTemplate[i].m_IsTrcHdr) //Not loading binary headers 
                {
                    m_TrcHdrNames.push_back(m_SegYTemplate[i].m_HdrName);

                    ++nHdrsToStore;
                }

            m_SeisTrcHdrs.resize(nTraces * nHdrsToStore);

            m_NumOfTrcs = nTraces;
            m_NumOfHdrs = nHdrsToStore;

            //Reserving space for all traces, assuming stacked data, that is, all source and receiver locations are unique.
            //However, if the data is prestack and sources and receivers are repeated, not all the reserved memory will be used. 
            //The extra meory will be released in that case.
            m_UniqSrcs.reserve(nTraces);
            m_UniqRcvs.reserve(nTraces);

        }

        catch (std::exception& e)
        {
            cErrWarnInfo::EWI(ewiERROR, e.what());
        }

    }

    void cSeisData::AddBinHdr(QString inName, QString inDesc, long inVal)
    {
        try {
            std::shared_ptr<CBinHdr> bhdr(new CBinHdr);

            bhdr->m_bhName = inName;
            bhdr->m_bhDesc = inDesc;
            bhdr->m_val = inVal;

            m_BinHdrs.push_back(*bhdr);
        }

        catch (exception& e)
        {
            cErrWarnInfo::EWI(ewiERROR, e.what());
        }

    }

    std::shared_ptr<CBinHdr> cSeisData::GetBinHdr(QString hdrName)
    {
        qint32 i;
        for (i = 0; i < m_BinHdrs.size(); ++i)
        {
            if (m_BinHdrs[i].m_bhName == hdrName)
            {
                std::shared_ptr<CBinHdr> uPtr = std::make_shared<CBinHdr>(m_BinHdrs[i]);
                return uPtr;
            }
        }
        return std::shared_ptr<CBinHdr>(nullptr);
    }

    bool cSeisData::SetEBCDIChdr(char* bufEbcdic)
    {
        const size_t COLS = 80;
        const size_t ROWS = 40;

        QString ebc32(bufEbcdic);

        uint8_t iter = 0;

        for (iter = 0; iter < ROWS; ++iter)
        {
            m_EbcdHdr.push_back(ebc32.mid((iter * COLS), COLS));

        }
        return true;
    }

    std::shared_ptr<QVector<QString>> cSeisData::GetEBCDIChdr()
    {
        std::shared_ptr<QVector<QString>> uPtr = std::make_shared<QVector<QString>>(m_EbcdHdr);
        return uPtr;
    }

    // Resets the pSeisData object. 
    // If user changes *.sgy or the template file after an initial loading has completed, all data should be cleaned.
    void cSeisData::Clear()
    {
        m_NumOfTrcs = 0;
        m_NumOfHdrs = 0;

        m_SeisTrcHdrs.clear();
        m_SeisTrcHdrs.squeeze();

        m_TrcHdrNames.clear();
        m_TrcHdrNames.squeeze();

        m_IsBinHdrAvail = false;
        m_TraceStride = 0;

        m_SgyFullFilePath = QString();
        m_DataName = QString();

        m_SgyTmplFilePath = QString();


        if (!m_UniqRcvs.isEmpty())
        {
            m_UniqRcvs.clear();
            m_UniqRcvs.squeeze();
        }

        if (!m_UniqSrcs.isEmpty())
        {
            m_UniqSrcs.clear();
            m_UniqSrcs.squeeze();
        }

        if (!m_BinHdrs.isEmpty())
        {
            m_BinHdrs.clear();
            m_BinHdrs.squeeze();
        }

        if (!m_EbcdHdr.isEmpty())
        {
            m_EbcdHdr.clear();
            m_EbcdHdr.squeeze();
        }

        if (!m_SortOrders.isEmpty())
        {
            m_SortOrders.clear();
            m_SortOrders.squeeze();
        }
    }

    bool cSeisData::SetHdrVal(QString name, quint64 TrcNum, double val)
    {
        qint16 hdrIndx = m_TrcHdrNames.indexOf(name);

        if (hdrIndx != -1) //Only proceed if header is found
        {
            quint64 aryIndx = ConvertIndxMatToRow(hdrIndx, TrcNum);
            m_SeisTrcHdrs[aryIndx] = val;
            return true;
        }

        else
        {
            cErrWarnInfo::EWI(ewiERROR, "No Trace Header found with the name " + name);
            return false;
        }

    }

    bool cSeisData::TmplteIsDblTrcHdr(QString name) const
    {
        for (auto i : m_SegYTemplate)
        {
            if (i.m_HdrName == name)
                return i.m_IsDblHdr;
        }

        cErrWarnInfo::EWI(ewiERROR, "No Trace Header found with the name " + name);
        return false;
    }

    QVariant cSeisData::GetHdrVal(qint64 indx, quint64 TrcNum) const
    {
        if (indx > m_TrcHdrNames.size())
            return QVariant();

        QString name = m_TrcHdrNames[indx];

        return GetHdrVal(name, TrcNum);

    }

    QVariant cSeisData::GetHdrVal(QString name, quint64 trcNum) const
    {
        qint16 row = m_TrcHdrNames.indexOf(name);

        if (row == -1)  //Header with this "name" not found
            return QVariant();

        quint64 indx = ConvertIndxMatToRow(row, trcNum);
        if (indx >= (quint64)m_SeisTrcHdrs.size())
            return QVariant();

        bool IsDbl = TmplteIsDblTrcHdr(name);

        if (IsDbl)
            return m_SeisTrcHdrs[indx].toDouble();
        else
            return m_SeisTrcHdrs[indx].toInt();


    }

    /* In the memory, the headers are stored
    * 
                 Header Layout (where "T1" is "trace # 1", "H1" is "header # 1")
                ----------------------------------------------------------------
                Column->    1         2          3     ...     n
                Row        T1H1      T2H1       T3H1          TnH1
                |          T1H2      T2H2       T3H2          TnH2
                v          T1H3      T2H3       T3H3          TnH3
                            .
                            .
                            .
                           T1Hn      T2Hn       T3Hn          TnHn
                -----------------------------------------------------------------

    This function returns the start index and length of a header "name" for all traces.
    Since all the trace headers are stored internally in contingent memory space, so 
    same header for each trace are adjacently stored. Using the returned index and length
    one could easily access same header for all traces.*/
    bool cSeisData::GetSameHdrForAllTrcs(QString name, qint64& strtIndx, quint64& lngth)
    {
        qint64 hdrIndx = m_TrcHdrNames.indexOf(name);

        if (hdrIndx != -1)
        {
            strtIndx = ConvertIndxMatToRow(hdrIndx, 0); //Getting header index of the first trace
            lngth = m_NumOfTrcs;
            return true;
        }

        return false;
    }

    //Returns an array of doubles suited for loading in OpenGL.
    //Coordinate are in the X, Z and Y order to be consistent with OpenGL coordinate system
    //Bytes are sequenced in X, Z, Y Attrib[1], Attrib[2], Attrib[n], X, Z, Y Attrib[1], Attrib[2], Attrib[n], ...
    //std::shared_ptr< QVector<qreal> > cSeisData::GetHeadersForGL(QString hEast, QString hNorth, QString hElev, QVector<QString> hAttrib, quint64 FrmTrc, quint64 ToTrc)
    //{
    //    if (ToTrc > m_NumOfTrcs || ToTrc == 0)
    //        ToTrc = m_NumOfTrcs;
    //    std::shared_ptr< QVector<qreal> > pvec(new QVector<qreal>);
    //    quint64 nTrcs = ToTrc - FrmTrc;
    //    quint32 nHdrs = hAttrib.size() + 3; //3 = hEast + hNorth + hElev
    //    try {
    //        pvec->reserve(nHdrs * nTrcs);
    //        //Collecting header index for each header at trace FrmTrc
    //        qint64 row, iEast, iNorth, iElev;
    //        row = m_TrcHdrNames.indexOf(hEast);
    //        if (row == -1)  //Header with this "name" not found
    //            return nullptr;
    //        iEast = ConvertIndxMatToRow(row, FrmTrc);
    //        row = m_TrcHdrNames.indexOf(hNorth);
    //        if (row == -1)  //Header with this "name" not found
    //            return nullptr;
    //        iNorth = ConvertIndxMatToRow(row, FrmTrc);
    //        row = m_TrcHdrNames.indexOf(hElev);
    //        if (row == -1)  //Header with this "name" not found
    //            return nullptr;
    //        iElev = ConvertIndxMatToRow(row, FrmTrc);
    //        QVector<quint64> iAttrib;
    //        iAttrib.reserve(hAttrib.size());
    //        for (auto i : hAttrib)
    //        {
    //            row = m_TrcHdrNames.indexOf(i);
    //            iAttrib.push_back(ConvertIndxMatToRow(row, FrmTrc));
    //        }
    //        //END : Collecting header indexes for each header at first trace
    //        quint32 sz = hAttrib.size();
    //        quint64 aIndx;
    //        for (quint64 trc = 0; (FrmTrc + trc) < ToTrc; ++trc)
    //        {
    //            pvec->push_back(m_SeisTrcHdrs[iEast + trc].toDouble());
    //            pvec->push_back(m_SeisTrcHdrs[iElev + trc].toDouble()); //Coordinate are in the X, Z and Y order to be consistent with OpenGL coordinate system
    //            pvec->push_back(m_SeisTrcHdrs[iNorth + trc].toDouble());
    //            for (quint16 h = 0; h < sz; ++h)
    //            {
    //                aIndx = iAttrib[h];
    //                pvec->push_back(m_SeisTrcHdrs[aIndx + trc].toDouble());
    //            }
    //        }
    //        return pvec;
    //    }//Try
    //    catch (exception e)
    //    {
    //        cErrWarnInfo::EWI(ewiERROR, e.what());
    //        return nullptr;
    //    }
    //}

    bool cSeisData::GetTraceGather(bool isSrcGather, quint32 GathNum, std::shared_ptr <cSeisGather> seisGth)
    {
        GathNum = GathNum - 1;
        //QString hEast, hNorth, hElev;

        if (isSrcGather && (GathNum < (quint32)m_UniqSrcs.size()))
        {
            //Traces in a CSG, will have different RCV positions
            seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc = std::make_shared<cLocXYZ> (m_UniqSrcs.at(GathNum));
            seisGth->m_GatherAttributes->m_HdrNames.push_back("XrcvGrpCoord");
            seisGth->m_GatherAttributes->m_HdrNames.push_back("YrcvGrpCoord");
            seisGth->m_GatherAttributes->m_HdrNames.push_back("RcvGrpElev");
        }

        else if (GathNum < (quint32)m_UniqRcvs.size())
        {
            //Traces in a CRG, will have different SRC positions
            seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc = std::make_shared<cLocXYZ>(m_UniqRcvs.at(GathNum));
            seisGth->m_GatherAttributes->m_HdrNames.push_back("XsrcCoord");
            seisGth->m_GatherAttributes->m_HdrNames.push_back("YsrcCoord");
            seisGth->m_GatherAttributes->m_HdrNames.push_back("SurfElevAtSrc");
        }

        try
        {
            cSegYReadWrite pSgyRedWrt;

            quint64 nGthTrcs = seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces.size();

            /// Reading the samples in to a local vector to be able to find min and max amplitudes 
            /// Then call the cColorMap with proper Min Max amplitude range.
            QVector<float> seisSamp;
            seisSamp.reserve(m_NumSamples * nGthTrcs);

            seisGth->m_SamplesRGB->reserve(m_NumSamples * nGthTrcs * 4); // * 4 for RGBA, one for each color channel

            long dataFrmt = GetBinHdr("DataFormat")->m_val;
                
            pSgyRedWrt.GetTraceSamples(dataFrmt, m_SgyFullFilePath, seisGth, seisSamp);
            
            auto result = std::minmax_element(seisSamp.begin(), seisSamp.end());

            //Normalizing the amplitudes
            float NormFact;
            if (abs(*result.first) >= abs(*result.second))
                NormFact = *result.first;
            else
                NormFact = *result.second;

            float NormMin = *result.first / NormFact;
            float NormMax = *result.second / NormFact;

            seisGth->m_ptrParent->m_ColorMap->UpdateAmpRange(NormMin, NormMax);

            NormFact /= m_dispNorm;

            seisGth->m_ptrParent->m_ColorMap->GetColor(seisSamp, seisGth->m_SamplesRGB, NormFact);
            
            return true;
        }//try

        catch (exception e)
        {
            cErrWarnInfo::EWI(ewiERROR, e.what());
            return false;
        }
    }

    bool cSeisData::GetStackedData(std::shared_ptr <cSeisGather> seisGth, QString& xHdr, QString& yHdr, QString& zHdr, quint64& frmTrc, quint64& toTrc)
    {
        try
        {
            /// These headers are used in the cGLWindow::EXP_RenderSeisGath function to read values from SegY file
            
            // ToDo check if the traces frmTrc and toTrc are both inclusive
            for (int i = frmTrc; i < toTrc; ++i)
            {
                seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces.push_back(i); //A gather for stacked data will have all the traces.
            }

            seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->xLoc = NAN; //Stacked data will not have a gather source point, so filling in a blank value.
            seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->yLoc = NAN; //Stacked data will not have a gather source point, so filling in a blank value.
            seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->zLoc = NAN; //Stacked data will not have a gather source point, so filling in a blank value.

            seisGth->m_GatherAttributes->m_HdrNames.push_back(xHdr);
            seisGth->m_GatherAttributes->m_HdrNames.push_back(yHdr);
            seisGth->m_GatherAttributes->m_HdrNames.push_back(zHdr);

            quint64 numTrcs = toTrc - frmTrc;

            if (numTrcs < 1)
                return false;

            /// Reading the samples in to a local vector to be able to find min and max amplitudes 
            /// Then call the cColorMap with proper Min Max amplitude range.
            QVector<float> seisSamp;
            seisSamp.reserve(m_NumSamples * numTrcs);

            seisGth->m_SamplesRGB->reserve(m_NumSamples * numTrcs * 4); // * 4 for RGBA, one for each color channel

            long dataFrmt = GetBinHdr("DataFormat")->m_val;

            cSegYReadWrite pSgyRedWrt;

            pSgyRedWrt.GetTraceSamples(dataFrmt, m_SgyFullFilePath, seisGth, seisSamp, frmTrc, toTrc);

            auto result = std::minmax_element(seisSamp.begin(), seisSamp.end());

            //Normalizing the amplitudes
            float NormFact;
            if (abs(*result.first) >= abs(*result.second))
                NormFact = *result.first;
            else
                NormFact = *result.second;

            float NormMin = *result.first;// / NormFact;
            float NormMax = *result.second;// / NormFact;

            seisGth->m_ptrParent->m_ColorMap->UpdateAmpRange(NormMin, NormMax);

            NormFact /= m_dispNorm;

            seisGth->m_ptrParent->m_ColorMap->GetColor(seisSamp, seisGth->m_SamplesRGB, NormFact);

            return true;
        }//try

        catch (exception e)
        {
            cErrWarnInfo::EWI(ewiERROR, e.what());
            return false;
        }
    }

    /// Function returns X, Z, Y coordinates for all the unique sources and receivers
    /// if getSrc = true, Unique sources are returned
    /// if getSrc = false, Unique receivers are returned
    std::shared_ptr< QVector<cLocXYZ>> cSeisData::GetSrcRcvs(bool getSrc)
    {

        std::shared_ptr< QVector<cLocXYZ>> loc;

        if (getSrc)
            loc = std::make_shared< QVector<cLocXYZ> >(m_UniqSrcs);

        else
            loc = std::make_shared< QVector<cLocXYZ> >(m_UniqRcvs);

        return loc;
    }

    //Fundction returns header values for loaded headers only
    std::shared_ptr<QVector<QVariant>> cSeisData::GetAllHdrsForATrc(quint64 TrcNum)
    {
        std::shared_ptr<QVector<QVariant>> tHdr(new QVector<QVariant>());

        quint32 sz = m_TrcHdrNames.size();

        tHdr->reserve(sz);

        quint64 valIndx = 0;

        for (quint32 i = 0; i < sz; ++i)
        {
            valIndx = ConvertIndxMatToRow(i, TrcNum);

            tHdr->push_back(m_SeisTrcHdrs[valIndx]);
        }

        return  tHdr;
    }

    void cSeisData::FindUniqueLoc(QString xHdr, QString yHdr, QString zHdr, QVector<cLocXYZ>& vLoc)
    {
        QString str;
        if (vLoc == m_UniqSrcs)
            str = "Sources";
        else
            str = "Receivers";


        bool found = false;
        qint64 xIndx = 0, yIndx = 0, zIndx = 0;
        quint64 lngth = 0;

        GetSameHdrForAllTrcs(xHdr, xIndx, lngth); // Getting byte locations for the headers. Hdr name is supplied and 2nd and 3rd parameters are returned filled.
        GetSameHdrForAllTrcs(yHdr, yIndx, lngth);
        GetSameHdrForAllTrcs(zHdr, zIndx, lngth);

        //Search workflow:
        //vLoc will store unique locations.
        //Each header is compared against all the elements in this vector
        //If a match is found, that trace number is appended to the  AssociTraces
        //If no match is found a new cLocXYZ object is created and appended to the vecLoc

        quint64 tr = 0;
        quint64 iloc = 0;

        std::shared_ptr<cLocXYZ> loc0 = std::make_shared<cLocXYZ>();

        //Storing the first item to start the loop
        loc0->xLoc = m_SeisTrcHdrs[xIndx + tr].toDouble();
        loc0->yLoc = m_SeisTrcHdrs[yIndx + tr].toDouble();
        loc0->zLoc = m_SeisTrcHdrs[zIndx + tr].toDouble();
        vLoc.push_back(*loc0); 

        for (; tr < lngth; ++tr) //looping on all traces
        {
            for (iloc = 0; iloc < vLoc.size(); ++iloc)
            {
                if (vLoc[iloc].IsWithinTol(
                    m_SeisTrcHdrs[xIndx + tr].toDouble(),
                    m_SeisTrcHdrs[yIndx + tr].toDouble(),
                    m_SeisTrcHdrs[zIndx + tr].toDouble()))
                {
                    vLoc[iloc].GatherTraces.push_back(tr); //Add trace number to the current location
                    found = true;
                    break;
                }
            }

            if (!found) //If location is out of tolerance, from any existing vecloc elements, a new cLocXYZ object is created
            {           //and the trace number id added to the new cLocXYZ object.

                std::shared_ptr<cLocXYZ> loc(new cLocXYZ());
                loc->xLoc = m_SeisTrcHdrs[xIndx + tr].toDouble();
                loc->yLoc = m_SeisTrcHdrs[yIndx + tr].toDouble();
                loc->zLoc = m_SeisTrcHdrs[zIndx + tr].toDouble();
                loc->GatherTraces.push_back(tr);
                vLoc.push_back(*loc);
            }

            found = false; //reseting 

        } //for (; tr < lngth; ++tr) //tr is the trace number

        vLoc.squeeze(); //Releasing extra allocated memory. This was allocated in the cSeisData::InitSeisTrc(quint64 nTraces) function.

        cErrWarnInfo::EWI(ewiINFOR, "Total unique " + str + " found = " + QString::number(vLoc.size() ));
    }

    void cSeisData::GetLocHdrs(QString xHdr, QString yHdr, QString zHdr, QVector<cLocXYZ>& vLoc)
    {
        qint64 xIndx = 0, yIndx = 0, zIndx = 0;
        quint64 lngth = 0;

        GetSameHdrForAllTrcs(xHdr, xIndx, lngth); // Getting byte locations for the headers. Hdr name is supplied and 2nd and 3rd parameters are returned filled.
        GetSameHdrForAllTrcs(yHdr, yIndx, lngth);
        GetSameHdrForAllTrcs(zHdr, zIndx, lngth);

        std::shared_ptr<cLocXYZ> loc0 = std::make_shared<cLocXYZ>();

        for(quint64 tr = 0; tr < lngth; ++tr) //looping on all traces
        {
            std::shared_ptr<cLocXYZ> loc(new cLocXYZ());
            loc->xLoc = m_SeisTrcHdrs[xIndx + tr].toDouble();
            loc->yLoc = m_SeisTrcHdrs[yIndx + tr].toDouble();
            loc->zLoc = m_SeisTrcHdrs[zIndx + tr].toDouble();
            loc->GatherTraces.push_back(tr);
            vLoc.push_back(*loc);

        } //for(quint64 tr = 0; tr < lngth; ++tr)

        vLoc.squeeze(); //Releasing extra allocated memory. This was allocated in the cSeisData::InitSeisTrc(quint64 nTraces) function.
    }

    /*DIVIDE Header by Header - NOT Implmented yet
    The below function is not needed currently. I cannot figure out a way to efficiently check header types for
    each of the three headers. Will look into it later

    ////Function will divide two headers and save the quotient
    ////All operations are done inplace
    ////dividend and quotient can be the same, in which the quotient overwrites dividends values
    ////All hedaers should of same length
    template <typename T, typename U, typename V>
    void cSeisData::Divide(QString dividend, QString divisor, QString quotient) //, quint64 frmHdr, quint64 toHdr)
    {
        quint64 dvndStrt, divsStrt, quoStrt, Lngth;

        std::shared_ptr<QVector<T>> divi =  GetAHdrForAllTrcs(dividend, dvndStrt, Lngth);
        std::shared_ptr<QVector<U>> divs =  GetAHdrForAllTrcs(divisor, divsStrt, Lngth);
        std::shared_ptr<QVector<V>> quot =  GetAHdrForAllTrcs(quotient, quoStrt, Lngth);

        for (quint64 i = 0; i < Lngth; ++i)
        {
            if(divs[divsStrt + i] != 0)

                quot[quoStrt + i] = divi[dvndStrt + i] / divs[divsStrt + i];
        }
    }
    */

    //Function will divide a header by a divisor
    //All operations are done inplace
    //All hedaers should of same length
    void cSeisData::Divide(QString Header, qint64 divisor)
    {

        qint64 hdrStrt;
        quint64 Lngth;

        GetSameHdrForAllTrcs(Header, hdrStrt, Lngth);

        for (quint64 i = 0; i < Lngth; ++i)
        {
            if (divisor != 0)
                m_SeisTrcHdrs[hdrStrt + i] = m_SeisTrcHdrs[hdrStrt + i].toDouble() / divisor;
        }

    }

    //Function will multiply a header by a scalar
    //All operations are done inplace
    //All hedaers should of same length
    void cSeisData::Multiply(QString Header, qint64 scalar)
    {
        quint64 Lngth;
        qint64 hdrStrt;

        GetSameHdrForAllTrcs(Header, hdrStrt, Lngth);

        // quint64 hdrIndx = m_TrcHdrNames.indexOf(Header);
        for (quint64 i = 0; i < Lngth; ++i)
        {
            m_SeisTrcHdrs[hdrStrt + i] = m_SeisTrcHdrs[hdrStrt + i].toDouble() * scalar;
        }
    }

    /// <summary>
    /// This function finds the min and max from the data;
    /// </summary>
    void cSeisData::UpdateMinMax()
    {

        for (auto i : m_UniqRcvs)
        {
            SetMinX(i.xLoc);
            SetMaxX(i.xLoc);
            SetMinY(i.yLoc);
            SetMaxY(i.yLoc);
            SetMinZ(i.zLoc);
            SetMaxZ(i.zLoc);
        }

        for (auto i : m_UniqSrcs)
        {
            SetMinX(i.xLoc);
            SetMaxX(i.xLoc);
            SetMinY(i.yLoc);
            SetMaxY(i.yLoc);
            SetMinZ(i.zLoc);
            SetMaxZ(i.zLoc);
        }

    }

    /// <summary>
    /// This function computes location average of two provided 3D points.
    /// It uses simple arithematic average method, that is, sum all values and divide
    /// by total number of values. This is done for each axis and for all data points.
    /// </summary>
    QVector3D cSeisData::ComputeLocAvrg(QVector3D& pt1, QVector3D& pt2)
    {
        qreal avgX = (pt1.x() + pt2.x()) / 2;
        qreal avgY = (pt1.y() + pt2.y()) / 2;
        qreal avgZ = (pt1.z() + pt2.z()) / 2;

        return QVector3D(avgX, avgY, avgZ);
    }

    /// <summary>
    /// This function computes location average for all the 3D points in the provided QVector3D
    /// It uses simple arithematic average method, that is, sum all values and divide
    /// by total number of values. This is done for each axis and for all data points.
    /// </summary>
    QVector3D cSeisData::ComputeLocAvrg(QVector<cLocXYZ>& vLocs)
    {

        /// Not creating and using a QVector3D for computations, 
        /// because QVector3D will take alot more fucntion calls
        /// e.g. setX() and .x() see below
        /// QVector3D avrg(0.0, 0.0, 0.0);
        /// avrg.setX(i.xLoc + avrg.x());
        /// Instead using the simple qreal avgX, avgY and avgZ data types.

        qreal avgX = 0.0;
        qreal avgY = 0.0;
        qreal avgZ = 0.0;
        quint64 numVal = vLocs.size();

        for (auto i : vLocs)
        {
            avgX = i.xLoc + avgX;
            avgY = i.yLoc + avgY;
            avgZ = i.zLoc + avgZ;
        }

        avgX = avgX / numVal;
        avgY = avgY / numVal;
        avgZ = avgZ / numVal;

        return QVector3D(avgX, avgY, avgZ);
    }
//}
