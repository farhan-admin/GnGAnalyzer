#pragma once
#include <QVector>


//namespace GnV //GnGViewer
//{
    //Class to store a source and/or receiver and the their respective trace numbers
    //Each cLocXYZ object stores one XYZ location and the trace numbers where the location is found.
    class cLocXYZ
    {
    public:
        cLocXYZ(double tolerance = 0.1) :
         xLoc(0.0), yLoc(0.0), zLoc(0.0), tol(tolerance)
        {
            tol2 = tol / 2;
        }

        double xLoc, yLoc, zLoc;

        double tol; // Tolerance is used while comparing numbers

        QVector<quint64> GatherTraces; //Traces linked with this source or receiver are stored in this QVector.

        bool operator==(const cLocXYZ& rhs) const;

        //Compares two locations. Returns true is difference is less then the tolerance on each axis.
        bool IsWithinTol(const double& x2, const double& y2, const double& z2);

    private:
        double tol2;

    };

//}

