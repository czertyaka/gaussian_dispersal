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
     bool CalculateDiffusionParameter(const size_t x, const size_t y);
     mt::t_emissionId m_emissionId; ///< current emission
     mt::t_sourceId m_sourceId; ///< source of current emission
     mt::t_dilutionFactors m_dilutions; ///< current dilution values
     mt::t_diffusionParameter m_sigma_z; ///< current vertical diffusion parameter, m
     const mt::t_distances* m_distances; ///< current distances

     mt::t_matrix& m_matrix;
     size_t m_xDim;
     size_t m_yDim;
};

#endif // DILUTIONSCALCULATOR_H
