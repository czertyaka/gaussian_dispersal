#ifndef LANDSCAPECALCULATOR_H
#define LANDSCAPECALCULATOR_H

#include "basecalculator.h"
#include "globaltypes.h"
#include "database.h"

class LandscapeCalculator : public BaseCalculator
{
public:
    LandscapeCalculator();
    virtual ~LandscapeCalculator();
    t_errorCode Execute() override;
private:
    void CalculateDistances(db::t_distanceMask& distanceMask);
    bool CalculateCorrections(db::t_srcTerrainCorrections& corrs, const mt::t_source& source);
    double CalcSlope(const db::t_landscape::const_iterator point, const mt::t_source &source) const;
    double CalcCorrection(const double slope) const;

    size_t m_xDim;
    size_t m_yDim;
};

#endif // LANDSCAPECALCULATOR_H
