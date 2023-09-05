#ifndef CBINHDR_H
#define CBINHDR_H

#include <QString>
using namespace std;

//namespace GnV //GnGViewer
//{

    class CBinHdr
    {
    public:
        QString m_bhName;  //Name of the binary header
        QString m_bhDesc;  //Short description
        long m_val;        //Value of the header

    };
//}
#endif // CBINHDR_H

