#ifndef DILUTIONSCALCULATOR_H
#define DILUTIONSCALCULATOR_H

#include "basecalculator.h"
#include "globaltypes.h"

class DilutionsCalculator : public BaseCalculator
{
public:
    DilutionsCalculator();
     t_returnCode Execute() override;
private:
     bool CalculateDilutions();
     bool CalculateDiffusionParameter();
     mt::t_emissionId m_emissionId; ///< current emission
     mt::t_sourceId m_sourceId; ///< source of current emission
     mt::t_dilutionFactors m_dilutions; ///< current dilution values
     double m_sigma_z[mt::SP_COUNT]; ///< current vertical diffusion parameter, m

     mt::t_matrix& m_matrix;
     size_t m_xDim;
     size_t m_yDim;
};

#endif // DILUTIONSCALCULATOR_H
