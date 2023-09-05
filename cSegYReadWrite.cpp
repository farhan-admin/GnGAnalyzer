#ifndef CSeisData_CPP
#define CSeisData_CPP

#include <QFileInfo>
#include "cSegYReadWrite.h"
#include "Common.h"

//namespace GnV //GnGViewer
//{


    cSegYReadWrite::cSegYReadWrite()
    {

    }

    //Populate the trace header values in the GUI and the cSeisData objects
    //Move this function back to CSegYDlg class to update the TraceHeader values in the table
    void cSegYReadWrite::PopulateTraceHdrs(std::shared_ptr<cSeisData> pSeisData, const QString& file)
    {
        QFileInfo finfo(file);

        if (finfo.exists())
        {
            if (pSeisData->m_IsBinHdrAvail)
            {
                //pSgyDlg->ui->lblSegYFileName->setText(finfo.absoluteFilePath());
                //readsegy(0);//Read the first SegY file.
            }
            else
                cErrWarnInfo::EWI(ewiERROR, "Binary header values are not available yet.");
        }
        else
            cErrWarnInfo::EWI(ewiERROR, "SegY file not found: " + finfo.baseName());
    }

    //Find Byte length of each trace (header and samples)
    void cSegYReadWrite::FindTraceStride(std::shared_ptr<cSeisData> pSeisData, quint64 numSampEachTrc, quint32 dataFormat)
    {
        /*Length of each trace, header and samples is:
         *
         * TraceStride = TRCHDR + (numSamplesInEachTrace x sampFormatSize)
         *
        */

        quint64 sampFormatSize = 0;

        switch (dataFormat) //switch case statement is for future expansion
        {
        case 1:
            sampFormatSize = 4; //IBM 4-byte floating point
            break;

        case 5:
            sampFormatSize = 4; //IEEE 4-byte floating point
            break;

        default:
        {
            cErrWarnInfo::EWI(1, "SegY sample data format: " + QString::number(dataFormat) + " is not supported in this version.");
            pSeisData->m_IsBinHdrAvail = false;
            return;
        }
        }

        pSeisData->m_TraceStride = (TRCHDR + (numSampEachTrc * sampFormatSize));
    }

    //Once user has updated the values of the template, this function stores the 
    //Binary Header and Trace Header values in to cSeisData object.
    void cSegYReadWrite::StoreSegYTemplate(std::shared_ptr<cSeisData> pSeisData,
        QTableWidget* BinTblWgt, //These pointers are needed to access user updated template values
        QTableWidget* TrcTblWgt) //These pointers are needed to access user updated template values
    {

        QString Name, Desc, Type;
        quint32 StrtByt, EndByt, NumByt;
        bool Load, IsTrcHdr;

        //Looping once for Binary header table and once for Trace Header table
        QTableWidget* tbls[] = { BinTblWgt, TrcTblWgt };
        QTableWidget* tblwidg;

        for (quint8 i = 0; i < 2; ++i)
        {
            tblwidg = tbls[i];
            quint16 nRows = tblwidg->rowCount();

            if (i == 0)
                IsTrcHdr = false;
            else
                IsTrcHdr = true;

            for (quint16 row = 0; row < nRows; ++row)
            {
                //Saving a copy of the final template values in the cSeisData object
                std::shared_ptr<cSegYTempla> HdrTmpl(new cSegYTempla);

                HdrTmpl->m_IsTrcHdr = IsTrcHdr;

                Name = tblwidg->item(row, 0)->text();
                HdrTmpl->m_HdrName = Name;

                if (IsDblTrcHdr(Name))
                    HdrTmpl->m_IsDblHdr = true;
                else
                    HdrTmpl->m_IsDblHdr = false;

                Type = tblwidg->item(row, 1)->text();
                HdrTmpl->m_HdrType = Type;

                Load = tblwidg->item(row, 3)->text().toUInt();
                HdrTmpl->m_HdrLoad = Load;

                StrtByt = tblwidg->item(row, 4)->text().toLong();
                HdrTmpl->m_StrtByte = StrtByt;

                EndByt = tblwidg->item(row, 5)->text().toLong();
                NumByt = (EndByt - StrtByt) + 1;

                if (NumByt == 2)
                    HdrTmpl->m_isLngtTwoByt = true;

                else if (NumByt == 4)
                    HdrTmpl->m_isLngtTwoByt = false;

                else //All data in the headers is suppose to be 2 or 4 byte long
                {
                    cErrWarnInfo::EWI(ewiINFOR, "In SegY template row number " + QString::number(row) + ", difference between binary header bytes "
                        + QString::number(EndByt) + " and " + QString::number(StrtByt) + " is more than 4 bytes. "
                        + "These bytes will not be read.");
                }

                Desc = tblwidg->item(row, 6)->text();
                HdrTmpl->m_HdrDesc = Desc;

                pSeisData->m_SegYTemplate.push_back(*HdrTmpl);
            }
        }
    }

    //Find Number of total traces in the SegY file. Using the file size and number of samples per trace etc.
    //number of samples per trace is taken from the binary header
    bool cSegYReadWrite::FindNumOfTraces(std::shared_ptr<cSeisData> pSeisData, quint64 fileSize)
    {
        /*Number of traces are computed as under:
         * File size should be
         *  = EbcdicHdr + BinaryHdr + (No.OfExtendedHdrs x SizeOfExtHdr(3200))
         *  + [{TrcHdr + (NoOfSamp x SampleByteSize)} x NumOfTraces]
         *
         * NumOfTraces =
         * [FileSize - EbcdicHdr + BinaryHdr + (No.OfExtendedHdrs x SizeOfExtHdr(3200))] / [{TrcHdr + (NoOfSamp x SampleByteSize)}]
         *
         *
        */
        if (pSeisData->m_IsBinHdrAvail)//Proceed only after the binary header values are available
        {
            quint64 NumOfTrcs = 0;
            quint32 sampFormatSize;
            quint32 numExtHdr = pSeisData->GetBinHdr(NumOfExtHdrs)->m_val;
            quint32 dataFormat = pSeisData->GetBinHdr(DataFormat)->m_val;
            quint32 numSamples = pSeisData->GetBinHdr(NumSampTrc)->m_val;
            pSeisData->m_NumSamples = numSamples;

            switch (dataFormat) //switch case statement is for future expansion
            {
            case 1:
                sampFormatSize = 4; //IBM 4-byte floating point
                break;

            case 5:
                sampFormatSize = 4; //IEEE 4-byte floating point
                break;

            default:
            {
                cErrWarnInfo::EWI(1, " SegY sample data format: " + QString::number(dataFormat) + " is not supported in this version.");
                pSeisData->m_IsBinHdrAvail = false;
                return false;
            }

            }
            NumOfTrcs = (fileSize - (EBCHDR + BINHDR + (numExtHdr * EBCHDR))) / (TRCHDR + (numSamples * sampFormatSize));


            if (NumOfTrcs == 0)
            {
                cErrWarnInfo::EWI(1, "ZERO traces read from the file. Something wrong with the selected file.");
                return false;
            }

            pSeisData->m_NumOfPredicTrc = NumOfTrcs;
            return true;
        }

        return false;
    }

    //Read and populate EBCDIC header in the cSeisData object and the GUI
    bool cSegYReadWrite::ReadEBCDIChdr(std::shared_ptr<cSeisData> pSeisData, QFile* inSegyfile)
    {
        try
        {
            if (!inSegyfile->open(QIODevice::ReadOnly))
                return false;

            QDataStream ebcStrm(inSegyfile);
            CSegYFunc sgyFunc;

            qint32 len;
            char bufEbcdic[EBCHDR];
            len = ebcStrm.readRawData(bufEbcdic, EBCHDR);

            if (len < 0)
            {
                cErrWarnInfo::EWI(1, "Error reading the TEXT (EBCDIC) header.");
                inSegyfile->close();
                return false;
            }

            sgyFunc.ebc2asc(EBCHDR, bufEbcdic);
            inSegyfile->close();

            if (!pSeisData->SetEBCDIChdr(bufEbcdic))
                return false;
        }//try 

        catch (exception& e)
        {
            cErrWarnInfo::EWI(1, e.what());
        }

        return true;
    }

    //Read and populate Binary header in the cSeisData object and the GUI
    bool cSegYReadWrite::ReadBinaryHeader(std::shared_ptr<cSeisData> pSeisData, QFile* inSegyfile)
    {
        CSegYFunc sgyFunc;
        char bufBinHdr[4];

        quint16 sz = pSeisData->m_SegYTemplate.size();
        if (sz == 0)
            return false;

        QString hdrName;
        QString desc;
        quint32 strtByt;
        qint32 numByt;
        quint32 iter;
        quint32 hdrVal = 0;

        if (!inSegyfile->open(QIODevice::ReadOnly))
            return false;

        try
        {
            for (iter = 0; iter < sz; ++iter)
            {
                if (!pSeisData->m_SegYTemplate[iter].m_IsTrcHdr) //Excluding the trace headers
                {
                    hdrName = pSeisData->m_SegYTemplate[iter].m_HdrName;
                    strtByt = pSeisData->m_SegYTemplate[iter].m_StrtByte;

                    if (pSeisData->m_SegYTemplate[iter].m_isLngtTwoByt)
                        numByt = 2;
                    else
                        numByt = 4;

                    if (numByt == 4 || numByt == 2) //Only Read if Byte size is 2 or 4.
                    {
                        inSegyfile->seek(strtByt - 1);

                        if (inSegyfile->read(bufBinHdr, numByt) != numByt)
                        {
                            pSeisData->m_IsBinHdrAvail = false;
                            inSegyfile->close();
                            cErrWarnInfo::EWI(1, "Binary Header Read Error.");
                            return false;
                        }

                        if (numByt == 4)
                            hdrVal = sgyFunc.convert4(bufBinHdr);
                        else
                            hdrVal = sgyFunc.convert2(bufBinHdr);

                        //qDebug()<< hdrName <<"\t" << strtByt << "\t" << hdrVal  << endl;
                        pSeisData->AddBinHdr(hdrName, desc, hdrVal);
                    }

                }//if(!pSeisData->m_SegYTemplate[iter].m_IsTrcHdr)

            } //for (iter = 0; iter < sz; ++iter)

            pSeisData->m_IsBinHdrAvail = true;
            inSegyfile->close();

            return true;
        } //try

        catch (exception& e)
        {
            cErrWarnInfo::EWI(1, e.what());
            return false;
        }

    }

    bool cSegYReadWrite::RunPreCheck(std::shared_ptr<cSeisData> pSeisData, quint64 frmTrc, quint64 toTrc)
    {

        quint64 numTrc = (toTrc - frmTrc);

        if (numTrc > pSeisData->m_NumOfPredicTrc)
            pSeisData->InitSeisTrc(pSeisData->m_NumOfPredicTrc);

        else if (numTrc > 0 && numTrc <= pSeisData->m_NumOfPredicTrc)
            pSeisData->InitSeisTrc(numTrc);

        else
        {
            cErrWarnInfo::EWI(1, "Number of traces requested for reading are invalid.");
            return false;
        }


        if (pSeisData->m_TraceStride == 0)
        {
            std::shared_ptr<CBinHdr> ptrSNumSampTrc = pSeisData->GetBinHdr(NumSampTrc);
            if (!ptrSNumSampTrc)
                return false;

            std::shared_ptr<CBinHdr> ptrDataFrmt = pSeisData->GetBinHdr(DataFormat);
            if (!ptrDataFrmt)
                return false;

            FindTraceStride(pSeisData, ptrSNumSampTrc->m_val, ptrDataFrmt->m_val);
        }

        return true;
    }

    void cSegYReadWrite::ExportHdrsToCSV(std::shared_ptr<cSeisData> pSeisData, QString fName, char delimiter)
    {
        Q_UNUSED(delimiter);
        Q_UNUSED(fName);
        Q_UNUSED(pSeisData);
        //QFile file(fName);
        //if (file.open(QIODevice::ReadWrite))
        //{
        //    QTextStream stream(&file);
        //
        //    for (int c = 0; c < pSeisData.size(); c++) // v1 and v2 better be same size!
        //        stream << v1[c] << "," << v2[c] << endl;
        //}
    }

    bool cSegYReadWrite::IsDblTrcHdr(QString& name)
    {

        int indx = m_lst.indexOf(name);

        if (indx != -1)
            return true;
        else
            return false;
    }

    // This function Reads Trace Headers
    bool cSegYReadWrite::ReadTrcHdr(std::shared_ptr<cSeisData> pSeisData, QString& inSegyfile, quint64& frmTrc, quint64& toTrc) //-1 means to read all traces in the file. While scanning user might want to scan limited traces
    {
        if ((toTrc == 0) || (toTrc > pSeisData->m_NumOfPredicTrc))
            toTrc = pSeisData->m_NumOfPredicTrc;

        RunPreCheck(pSeisData, frmTrc, toTrc);

        if (!pSeisData->m_IsBinHdrAvail || (pSeisData->m_NumOfPredicTrc == 0))
            return false;


        QFile inFile(inSegyfile);
        if (!inFile.open(QIODevice::ReadOnly))
        {
            cErrWarnInfo::EWI(ewiERROR, inFile.error());
            return false;
        }
            

        CSegYFunc sgyFunc;

        quint64 loc;
        quint64 trcStrid = pSeisData->m_TraceStride;
        quint32 fileHdr = EBCHDR + BINHDR;

        QByteArray bytArry;
        qint32 hdrVal = -9999;

        /// <summary>
        /// Creating a local copy for performance and future multi threading implementation.
        /// Cannot use the cSeisData.m_TrcHdrNames because more byte length information is also required for header conversion
        /// </summary>
        QVector<cSegYTempla> LclTmpl; 
        LclTmpl.reserve( pSeisData->m_SegYTemplate.size() );
        
        for (auto hdr : pSeisData->m_SegYTemplate) ///Extracting only the trace headers
        {
            if (hdr.m_IsTrcHdr)
                LclTmpl.push_back(hdr);
        }

        LclTmpl.squeeze();
        quint32 numHdrs = LclTmpl.size();

        for (quint64 tr = frmTrc; tr < toTrc; ++tr)
        {
            loc = (tr * trcStrid) + fileHdr;

            if (!inFile.seek(loc))
                return false; //Seeking to trace location failed.

            bytArry = inFile.read(TRCHDR); //Reading the whole trace header at once to avoid multiple file read calls

            if (bytArry.at(0) == -1)
                return false;  //If file->read returns error its probably reached end of file



            for (quint32 hdr = 0; hdr < numHdrs; ++hdr)
            {
                if (LclTmpl[hdr].m_HdrLoad)
                {

                    if (LclTmpl[hdr].m_isLngtTwoByt)
                        hdrVal = sgyFunc.convert2(bytArry.mid((LclTmpl[hdr].m_StrtByte - 1), 2));

                    else
                        hdrVal = sgyFunc.convert4(bytArry.mid((LclTmpl[hdr].m_StrtByte - 1), 4));

                    if (!pSeisData->SetHdrVal(LclTmpl[hdr].m_HdrName, tr, hdrVal))
                        break; //Break if header value cannot be set for any reason

                }
            }

        } // for (quint64 tr = frmTrc; tr < toTrc; ++tr)

        ApplySpatialScalars(pSeisData);
        ApplyDepthScalars(pSeisData);

        inFile.close();

        return true;

    }

    //OVERLOADED fucntion. Reads the provided number of traces. 
    bool cSegYReadWrite::GetTraceSamples(long& DataFrmt, QString& strSegyfile, std::shared_ptr <cSeisGather> seisGth, QVector<float>& vecSamp, quint64 frmTrc, quint64 toTrc)
    {
        try
        {

            switch (DataFrmt) //Using switch case, for future expansion for including other sample formats
            {
            case 1: //IBM 4-byte floating point
            {
                TRACEReader_IBM2IEEE(strSegyfile, seisGth, frmTrc, toTrc, vecSamp);

                break;
            }

            case 5: //IEEE 4-byte floating point
            {

                TRACEReader_RawBytes2IEEE(strSegyfile, seisGth, frmTrc, toTrc, vecSamp);

                break;
            }

            } //switch

        }

        catch (exception& e)
        {
            return false;
        }

        return true;
    }

    //OVERLOADED fucntion. Reads all traces for the whole gather. Inteneded to be used for pre-stacked data. 
    bool cSegYReadWrite::GetTraceSamples(long& DataFrmt, QString& strSegyfile, std::shared_ptr <cSeisGather> seisGth, QVector<float>& vecSamp)
    {
        try
        {
                        
            switch (DataFrmt) //Using switch case, for future expansion for including other sample formats
            {
                case 1: //IBM 4-byte floating point
                {
                    quint64 frmTrc = 0;
                    quint64 toTrc = seisGth->m_ptrParent->m_NumOfTrcs;

                    TRACEReader_IBM2IEEE(strSegyfile, seisGth, frmTrc, toTrc, vecSamp);

                    break;
                }

                case 5: //IEEE 4-byte floating point
                {
                    quint64 frmTrc = 0;
                    quint64 toTrc = seisGth->m_ptrParent->m_NumOfTrcs;

                    TRACEReader_RawBytes2IEEE(strSegyfile, seisGth, frmTrc, toTrc, vecSamp);

                    break;
                }

            } //switch

        }

        catch (exception& e)
        {
            return false;
        }

        return true;
    }

    bool cSegYReadWrite::TRACEReader_IBM2IEEE(QString& strSegyfile, std::shared_ptr <cSeisGather> seisGth, quint64& frmTrc, quint64& toTrc, QVector<float>& vecSamp)
    {
        const uint SAMPLESIZE = 4;  //Sample size, Each sample is 4 bytes wide in the Case 1

        //unsigned char byt[SAMPLESIZE];

        QFile inSegyfile(strSegyfile);

        if (!inSegyfile.open(QIODevice::ReadOnly))
            return false;

        QByteArray rawByteArray;
        float smp = 0.0f;
        quint64 trStrtByt = 0;

        quint64 TraceStride = seisGth->m_ptrParent->m_TraceStride;
        quint64 NumSamples = seisGth->m_ptrParent->m_NumSamples;

        CSegYFunc sgyFnc;

        quint64 curTrc = 1; //Current trace

        for (quint64 trc = frmTrc; trc < toTrc; ++trc)
        {
            if (trc > (quint64)seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces.size() - 1 )
                break;

            curTrc = seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces[trc];

            trStrtByt = (TraceStride * curTrc) + EBCHDR + BINHDR + TRCHDR; // This lands to the first sample location

            inSegyfile.seek(trStrtByt);

            rawByteArray = inSegyfile.read(NumSamples * SAMPLESIZE);

            Q_ASSERT(!rawByteArray.isEmpty());  //This should not be empty

            if (rawByteArray.isEmpty())
            {
                cErrWarnInfo::EWI(ewiERROR, "Error reading samples of trace number: " + QString::number(curTrc) );
                return false;
            }

            // NOTE:
            // The "=" in the "<=" for loop, is for reading the last sample in a trace.
            // If "=" is removed, for loop will cuts the last sample
            // Do not remove the "=" in the "<="
            for (quint64 s = 0; (s + SAMPLESIZE) <= (quint64)rawByteArray.size(); s += SAMPLESIZE)
            {
                sgyFnc.ibm2ieee(&smp, rawByteArray.mid(s, SAMPLESIZE));
                vecSamp.push_back(smp);
            }
        }

        return false;
    }

    bool cSegYReadWrite::TRACEReader_RawBytes2IEEE(QString& strSegyfile, std::shared_ptr <cSeisGather> seisGth, quint64& frmTrc, quint64& toTrc, QVector<float>& vecSamp)
    {

        const uint SAMPLESIZE = 4;  //Sample size, Each sample is 4 bytes wide in the Case 1

        unsigned char byt[SAMPLESIZE];

        QFile inSegyfile(strSegyfile);

        if (!inSegyfile.open(QIODevice::ReadOnly))
            return false;

        QByteArray rawByteArray;
        float smp = 0.0f;
        quint64 trStrtByt = 0;

        quint64 TraceStride = seisGth->m_ptrParent->m_TraceStride;
        quint64 NumSamples = seisGth->m_ptrParent->m_NumSamples;

        quint64 curTrc = 1; //Current trace

        for (quint64 trc = frmTrc; trc < toTrc; ++trc)
        {
            if (trc > (quint64)seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces.size() - 1)
                break;

            curTrc = seisGth->m_GatherAttributes->m_ptrGatherLocAndAssoTrc->GatherTraces[trc];

            trStrtByt = (TraceStride * curTrc) + EBCHDR + BINHDR + TRCHDR; // This lands to the first sample location

            inSegyfile.seek(trStrtByt);

            rawByteArray = inSegyfile.read(NumSamples * SAMPLESIZE);

            Q_ASSERT(!rawByteArray.isEmpty());  //This should not be empty

            if (rawByteArray.isEmpty())
            {
                cErrWarnInfo::EWI(ewiERROR, "Error reading samples of trace number: " + QString::number(curTrc));
                return false;
            }

            // NOTE:
            // The "=" in the "<=" for loop, is for reading the last sample in a trace.
            // If "=" is removed, for loop will cuts the last sample
            // Do not remove the "=" in the "<="
            for (quint64 s = 0; (s + SAMPLESIZE) <= (quint64)rawByteArray.size(); s += SAMPLESIZE)
            {
                // Byte to float code segment taken from 
                //https://stackoverflow.com/questions/3991478/building-a-32-bit-float-out-of-its-4-composite-bytes

                byt[3] = rawByteArray.data()[s];
                byt[2] = rawByteArray.data()[s + 1];
                byt[1] = rawByteArray.data()[s + 2];
                byt[0] = rawByteArray.data()[s + 3];

                memcpy(&smp, &byt, SAMPLESIZE);

                vecSamp.push_back(smp);
            }
        }

        return false;
    }

    //Applying Coordinated scalars
    void cSegYReadWrite::ApplySpatialScalars(std::shared_ptr<cSeisData> pSeisData)
    {
        qint64 scalar = pSeisData->GetHdrVal(ScalCoord, 0).toInt(); // scalar value is taken from the first header

        if ((scalar == 1) || (scalar == 0)) //No scaling needed in these cases.
        {
            cErrWarnInfo::EWI(ewiWARNI, "Spatial scaling header is " + QString::number(scalar) + ". No scaling was applied to the coordinate headers.");
            return;
        }

        if (scalar > 1)
        {
            pSeisData->Multiply(XsrcCoord, scalar);
            pSeisData->Multiply(YsrcCoord, scalar);
            pSeisData->Multiply(XrcvGrpCoord, scalar);
            pSeisData->Multiply(YrcvGrpCoord, scalar);
        }
        else
        {
            scalar = std::abs(scalar);//-ve scalar in SegY header only means division. I does not mean division by a -ve divisor
            pSeisData->Divide(XsrcCoord, scalar);
            pSeisData->Divide(YsrcCoord, scalar);
            pSeisData->Divide(XrcvGrpCoord, scalar);
            pSeisData->Divide(YrcvGrpCoord, scalar);
        }

    }

    //Applying Depth scalars
    void cSegYReadWrite::ApplyDepthScalars(std::shared_ptr<cSeisData> pSeisData)
    {
        qint64 scalar = pSeisData->GetHdrVal(ScalElevDepth, 0).toInt(); // scalar value is taken from the first header

        if ((scalar == 1) || (scalar == 0)) //No scaling needed in these cases.
        {
            cErrWarnInfo::EWI(ewiWARNI, "Depth scaling header is " + QString::number(scalar) + ". No scaling was applied to the depth headers.");
            return;
        }

        if (scalar > 1)
        {
            pSeisData->Multiply(RcvGrpElev, scalar);
            pSeisData->Multiply(SurfElevAtSrc, scalar);
            pSeisData->Multiply(SrcDpthBlowSurf, scalar);
            pSeisData->Multiply(DatmElevAtRcvGrp, scalar);
            pSeisData->Multiply(DatmElevAtSrc, scalar);
            pSeisData->Multiply(WaterDpthAtSrcGrp, scalar);
            pSeisData->Multiply(WaterDpthArRcvGrp, scalar);
        }
        else
        {
            scalar = std::abs(scalar); //-ve scalar in SegY header only means division. I does not mean division by a -ve divisor
            pSeisData->Divide(RcvGrpElev, scalar);
            pSeisData->Divide(SurfElevAtSrc, scalar);
            pSeisData->Divide(SrcDpthBlowSurf, scalar);
            pSeisData->Divide(DatmElevAtRcvGrp, scalar);
            pSeisData->Divide(DatmElevAtSrc, scalar);
            pSeisData->Divide(WaterDpthAtSrcGrp, scalar);
            pSeisData->Divide(WaterDpthArRcvGrp, scalar);
        }

    }


//}
#endif // CSeisData_CPP
