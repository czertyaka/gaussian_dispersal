#ifndef TERRAINCORRECTIONSCALCULATOR_H
#define TERRAINCORRECTIONSCALCULATOR_H

#include "basecalculator.h"
#include "globaltypes.h"
#include "database.h"

class TerrainCorrectionsCalculator : public BaseCalculator
{
public:
    TerrainCorrectionsCalculator();
    virtual ~TerrainCorrectionsCalculator();
    t_errorCode Execute() override;
private:
    bool CalculateCorrections(db::t_sourceTerrainCorrections& corrs, const mt::t_source& source);
    double CalcSlope(const db::t_landscape::const_iterator point, const mt::t_source &source) const;
    double CalcCorrection(const double slope) const;

    size_t m_xDim;
    size_t m_yDim;
};

#endif // TERRAINCORRECTIONSCALCULATOR_H
