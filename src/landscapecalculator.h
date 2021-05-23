#ifndef LANDSCAPECALCULATOR_H
#define LANDSCAPECALCULATOR_H

#include "basecalculator.h"
#include "globaltypes.h"

class LandscapeCalculator : public BaseCalculator
{
public:
    LandscapeCalculator();
    virtual ~LandscapeCalculator();
    t_errorCode Execute() override;
private:
    void CalculateDistances(const size_t srcId, mt::t_distances& distances);
    bool CalculateCorrections(mt::t_terrainCorrections& corrs, const mt::t_source& source);
    double CalcSlope(const double x, const double y, const mt::t_source &source) const;
    double CalcCorrection(const double slope) const;

    size_t m_xDim;
    size_t m_yDim;
};

#endif // LANDSCAPECALCULATOR_H
