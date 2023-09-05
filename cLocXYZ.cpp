#include <QtGlobal>
#include "cLocXYZ.h"

//namespace GnV //GnGViewer
//{

    bool cLocXYZ::operator==(const cLocXYZ& rhs) const
    {
        if (qFuzzyCompare(this->xLoc, rhs.xLoc) &&
            qFuzzyCompare(this->yLoc, rhs.yLoc) &&
            qFuzzyCompare(this->zLoc, rhs.zLoc))
            return true;

        else
            return false;
    }


    bool cLocXYZ::IsWithinTol(const double& x2, const double& y2, const double& z2)
    {
        if (((std::abs(this->xLoc - x2) + tol2) <= tol) &&
            ((std::abs(this->yLoc - y2) + tol2) <= tol) &&
            ((std::abs(this->zLoc - z2) + tol2) <= tol))
            return true;

        else
            return false;

    };
//}