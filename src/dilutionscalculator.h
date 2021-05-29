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
     bool CalculateDepletions(const size_t x, const size_t y);
     bool CalculateSums(const size_t x, const size_t y, double& coldSum, double& warmSum);
     double CalculateAngle(const size_t x, const size_t y) const;
     bool InitWindSpeedsCache();
     bool InitDecayRate();
     double m_distance; ///< distance between source and current point
     mt::t_emissionId m_emissionId; ///< current emission
     mt::t_sourceId m_sourceId; ///< source of current emission
     t_epsg4326coord m_sourceCoord; ///< coordinate of current emission source
     double m_sourceHeight; ///< current source height
     mt::t_dilutionFactors m_dilutions; ///< current dilution values
     mt::t_diffusionParameter m_sigma_z; ///< current vertical diffusion parameter, m
     ///< depletion function for current point
     std::array<double , mt::SP_COUNT> m_depletionsCold;
     std::array<double , mt::SP_COUNT> m_depletionsWarm;
     ///< wind speed at emission height cache for each j and z0
     mt::t_windSpeedsAtHeight m_windSpeedsAtHeightCold;
     mt::t_windSpeedsAtHeight m_windSpeedsAtHeightWarm;
     double m_decayRate; ///< current decay rate, sec^-1

     const mt::t_distances* m_distances; ///< current distances

     mt::t_matrix& m_matrix;
     size_t m_xDim;
     size_t m_yDim;
};

#endif // DILUTIONSCALCULATOR_H
