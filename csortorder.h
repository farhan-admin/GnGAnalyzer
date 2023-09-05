#ifndef CSORTORDER_H
#define CSORTORDER_H

//namespace GnV //GnGViewer
//{

    class CSortOrder //Struct to store sorting order.
    {

    protected:
        QString m_SortOrderName;
        QVector<QString> m_SortingHeaders;
        QString m_GatherSplitHdr;

        //QVector storing the sorted trace numbers
        QVector< QVector<quint64> > m_TrcSeq; //2D Vector array storing Trace numbers
                                              //First dimension stores number of gathers
                                              //Second stores the sequence of traces within each gather for a sort order
    };
//}

#endif // CSORTORDER_H
