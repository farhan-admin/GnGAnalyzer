#ifndef CERRWARNINFO_H
#define CERRWARNINFO_H

#include <exception>
#include <iostream>
#include <QString>
#include <Qtglobal>
#include <QtDebug>

#define ewiERROR  0
#define ewiWARNI  1
#define ewiINFOR  2

//namespace GnV //GnGViewer
//{
    class cErrWarnInfo
    {

    public:
        static void EWI(short msgtype, QString msg);
        static void EWI(short msgtype, uint err);
        static void PrintGatherSamples(std::shared_ptr<QVector<qreal>> gth, uint nTraces, uint nSamples);

    private:

        static void ReportError(QString msg);
        static void ReportWarn(QString msg);
        static void ReportInfo(QString msg);
        static void ReportDefault(QString msg);
        static QString FileErrorDes(uint err);
    };
//}

#endif // CERRWARNINFO_H

