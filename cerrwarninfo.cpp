#include <iostream>
#include "cerrwarninfo.h"



//namespace GnV //GnGViewer
//{

    void cErrWarnInfo::EWI(short msgtype, QString msg)
    {
        switch (msgtype) {

        case 0://ERROR
            ReportError(msg);
            break;

        case 1://WARNING
            ReportWarn(msg);
            break;

        case 2://INFORMATION
            ReportInfo(msg);
            break;

        default:
            ReportDefault(msg);

        }

    }

    //File IO errors
    void cErrWarnInfo::EWI(short msgtype, uint err)
    {
        EWI(msgtype, FileErrorDes(err));
    }

    void cErrWarnInfo::ReportError(QString msg)
    { 
        qInfo() << "ERROR: " + msg;
        //std::cout << "ERROR: " <<  msg.toStdString();
    }

    void cErrWarnInfo::ReportWarn(QString msg)
    {
        qInfo() << "WARN: " + msg;
        //std::cout << "WARN: " << msg.toStdString();
    }

    void cErrWarnInfo::ReportInfo(QString msg)
    {
        qInfo() << "INFO: " + msg;
        //std::cout << "INFO: " << msg.toStdString();
    }

    void cErrWarnInfo::ReportDefault(QString msg)
    {
        qInfo() << "DEFAULT: " + msg;
        //std::cout << "DEFAULT: " << msg.toStdString();
    }


    //https://doc.qt.io/qt-5/qfiledevice.html#FileHandleFlag-enum
    //QFileDevice::NoError	0	No error occurred.
    //QFileDevice::ReadError	1	An error occurred when reading from the file.
    //QFileDevice::WriteError	2	An error occurred when writing to the file.
    //QFileDevice::FatalError	3	A fatal error occurred.
    //QFileDevice::ResourceError	4	Out of resources(e.g., too many open files, out of memory, etc.)
    //QFileDevice::OpenError	5	The file could not be opened.
    //QFileDevice::AbortError	6	The operation was aborted.
    //QFileDevice::TimeOutError	7	A timeout occurred.
    //QFileDevice::UnspecifiedError	8	An unspecified error occurred.
    //QFileDevice::RemoveError	9	The file could not be removed.
    //QFileDevice::RenameError	10	The file could not be renamed.
    //QFileDevice::PositionError	11	The position in the file could not be changed.
    //QFileDevice::ResizeError	12	The file could not be resized.
    //QFileDevice::PermissionsError	13	The file could not be accessed.
    //QFileDevice::CopyError	14	The file could not be copied.
    QString cErrWarnInfo::FileErrorDes(uint err)
    {
        switch (err)
        {
        case 0: //QFileDevice::NoError     0
            return ("No error occurred.");
            break;

        case 1: // QFileDevice::ReadError	1	
            return("An error occurred when reading from the file.");
            break;

        case 2: //        QFileDevice::WriteError	2	
            return("An error occurred when writing to the file.");
            break;

        case 3: //       QFileDevice::FatalError	3	
            return("A fatal error occurred.");
            break;

        case 4://QFileDevice::ResourceError	4	
            return("Out of resources(e.g., too many open files, out of memory, etc.");
            break;

        case 5://QFileDevice::OpenError	5	
            return("The file could not be opened.");
            break;

        case 6://QFileDevice::AbortError	6	
            return("The operation was aborted.");
            break;

        case 7://QFileDevice::TimeOutError	7	
            return("A timeout occurred.");
            break;

        case 8://QFileDevice::UnspecifiedError	8	
            return("An unspecified error occurred.");
            break;

        case 9://QFileDevice::RemoveError	9	
            remove("The file could not be removed.");
            break;

        case 10://QFileDevice::RenameError	10	
            return("The file could not be renamed.");
            break;

        case 11://QFileDevice::PositionError	11	
            return("The position in the file could not be changed.");
            break;

        case 12://QFileDevice::ResizeError	12	
            return("The file could not be resized.");
            break;

        case 13://QFileDevice::PermissionsError	13	
            return("The file could not be accessed.");
            break;

        case 14://QFileDevice::CopyError	14	
            return("The file could not be copied.");
            break;
        }
 
        return QString();
    }

    void cErrWarnInfo::PrintGatherSamples(std::shared_ptr<QVector<qreal>> gth, uint nTraces, uint nSamples)
    {
        QString str;
        for (uint t = 0; t < nTraces; ++t)
        {
            for (uint s = 0; s < nSamples; ++s)
            {
                str = str + QString::number(gth->at((t * nSamples) + s)) + ",";
            }

            qDebug().noquote() << str + "Trace # " + QString::number(t);
            str = "";
        }
    }


//}



        //void cErrWarnInfo::EWI(short msgtype, QString msg, exception e)
    //{

    //    Q_UNUSED(e); // later we Will fugure out how to show the exception message

    //    switch (msgtype) {

    //    case 1://ERROR
    //        ReportError(msg);
    //        break;

    //    case 2://WARNING
    //        ReportWarn(msg);
    //        break;

    //    case 3://INFORMATION
    //        ReportInfo(msg);
    //        break;

    //    default:
    //        ReportDefault(msg);

    //    }

    //}
