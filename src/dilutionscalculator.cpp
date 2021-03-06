#include "dilutionscalculator.h"
#include "database.h"
#include "datainterface.h"

#include <cmath>

DilutionsCalculator::DilutionsCalculator() :
    m_matrix(m_db.Matrix())
{

}

BaseCalculator::t_returnCode DilutionsCalculator::Execute()
{   
    bool result = true;

    m_xDim = m_db.CoordSet().lon.size();
    m_yDim = m_db.CoordSet().lat.size();

    m_db.Dilutions().clear();
    m_db.Concentrations().clear();

    // calculating dilutions for each emission
    for (auto iter = m_db.Emissions().begin(); iter != m_db.Emissions().end(); ++iter)
    {
        // update ids
        m_emissionId = iter->first;
        m_sourceId = iter->second.source;

        // and source characteristics
        m_sourceCoord = m_db.Sources().find(m_sourceId)->second.coordinates;
        m_sourceHeight = m_db.Sources().find(m_sourceId)->second.height;

        // update vector
        m_dilutions.clear();
        m_dilutions.Init(m_db.Landscape());
        std::fill(m_dilutions.begin(), m_dilutions.end(), 0);

        // initalize wind speed cache and decay rate
        InitWindSpeedsCache();
        InitDecayRate();

        // start calculation
        MY_LOG("starting calculation dilution factor for " << iter->second);
        if (CalculateDilutions())
        {
            // add to db
            m_db.Dilutions().insert(std::make_pair(m_emissionId, m_dilutions));
        }
        else
        {
            MY_LOG("failed to calculate dilutions for " << iter->second);
            m_db.Dilutions().erase(iter->first);
            result &= false;
            continue;
        }
    }

    // update concecntrations table due to dilution factors
    // successful calculation
    m_db.Concentrations().Update();

    return result ? OK : ERROR;
}

/**
 * @brief DilutionsCalculator::CalculateDilutions Calculates
 *        dilutions array according to current @ref m_emissionId
 * @return
 */
bool DilutionsCalculator::CalculateDilutions()
{
    bool result = true;

    double coldFraction = static_cast<double>(m_matrix.MCold) / (m_matrix.MCold + m_matrix.MWarm);
    double warmFraction = 1 - coldFraction;

    mt::t_terrainCorrections& corrs = m_db.TerrainCorrections().find(m_sourceId)->second;
    m_distances = &m_db.Distances().find(m_sourceId)->second;

    size_t errorX = 0;
    size_t errorY = 0;
    for (size_t y = 0; y < m_yDim; ++y)
    {
        for (size_t x = 0; x < m_xDim; ++x)
        {
            errorX = x;
            errorY = y;

            // skip if we're far too beyond gaussian model or in the source!
            double distance = m_distances->at(x, y);
            if (distance > gaussian_model_limit || distance == 0)
            {
                continue;
            }

            // / ( Rn * x ) from formula
            m_distance = m_distances->at(x, y);
            double commonFactor = 2. * m_matrix.N / ( std::pow(2 * M_PI, 1.5) * corrs.at(x, y) * m_distance );

            // calculate diffusion parameter for current point
            if (!CalculateDiffusionParameter(x, y))
            {
                result &= false;
                break;
            }

            // calculate depletion functions
            CalculateDepletions(x, y);

            // calculate sums
            double coldSum = 0;
            double warmSum = 0;

            if (!CalculateSums(x, y, coldSum, warmSum))
            {
                result &= false;
                break;
            }

            // finally...
            double value = commonFactor * (coldFraction * coldSum + warmFraction * warmSum);
            m_dilutions.at(x, y) = value;
        }
    }

    if (!result)
    {
        MY_LOG("failed to calculate dilution factor for " << m_db.Landscape().at(errorX, errorY).coord);
    }

    return result;
}

bool DilutionsCalculator::CalculateDiffusionParameter(const size_t x, const size_t y)
{
    m_sigma_z.fill(0);

    const mt::t_diffusionParameter& max = m_db.MaxDiffusionPatameters();

    // get current distance
    double r = m_distances->at(x, y);

    // and landscape type
    mt::t_microrelief mr = m_db.Landscape().at(x, y).microrelief;

    // aaand coeffs for f(z0, x)
    const char rough = m_db.Roughness()[mr];
    const double c1 = m_db.FCoeffs().c1[mr];
    const double c2 = m_db.FCoeffs().c2[mr];
    const double d1 = m_db.FCoeffs().d1[mr];
    const double d2 = m_db.FCoeffs().d2[mr];

    // calc f(z0, x)
    const double f = rough <= 10 ?
                std::log( c1 * std::pow(r, d1) / (1 + c2 * std::pow(r, d2) ) ) :
                std::log( c1 * std::pow(r, d1) * (1 + 1 / ( c2 * std::pow(r, d2) ) ) );

    // loop over smith parameters
    for (size_t sp = mt::SP_A; sp < mt::SP_COUNT; ++sp)
    {
        // coeggs for g(x)
        const double a1 = m_db.GCoeffs().a1[sp];
        const double a2 = m_db.GCoeffs().a2[sp];
        const double b1 = m_db.GCoeffs().b1[sp];
        const double b2 = m_db.GCoeffs().b2[sp];

        // calc g(x)
        const double g = a1 * std::pow(r, b1) / (1 + a2 * std::pow(r, b2));

        m_sigma_z[sp] = f * g < max[sp] ? f * g : max[sp];
    }

    return true;
}

bool DilutionsCalculator::CalculateDepletions(const size_t x, const size_t y)
{
    mt::t_microrelief mr = m_db.Landscape().at(x, y).microrelief;

    for (size_t sp = mt::SP_A; sp < mt::SP_COUNT; ++sp)
    {
        m_depletionsCold[sp] = std::exp( -m_decayRate * m_distance /
            m_windSpeedsAtHeightCold.w[sp][mr] );
        m_depletionsWarm[sp] = std::exp( -m_decayRate * m_distance /
            m_windSpeedsAtHeightWarm.w[sp][mr] );
    }

    return true;
}

bool DilutionsCalculator::CalculateSums(const size_t x, const size_t y, double &coldSum, double& warmSum)
{
    double angle = CalculateAngle(x, y);
    mt::t_microrelief mr = m_db.Landscape().at(x, y).microrelief;

    coldSum = 0;
    warmSum = 0;

    for (size_t sp = mt::SP_A; sp < mt::SP_COUNT; ++sp)
    {
        for (size_t ws = 0; ws < m_db.Matrix().K; ++ws)
        {
            if (m_sigma_z[sp] * m_windSpeedsAtHeightCold.w[sp][mr] != 0)
            {
                coldSum += m_db.Matrix().interpCold[sp][ws](angle) / ( m_sigma_z[sp] *
                    m_windSpeedsAtHeightCold.w[sp][mr] ) * m_depletionsCold[sp] *
                    std::exp( - (m_sourceHeight * m_sourceHeight) / (2 * m_sigma_z[sp] *
                    m_sigma_z[sp]) );
            }
            if (m_sigma_z[sp] * m_windSpeedsAtHeightWarm.w[sp][mr] != 0)
            {
                warmSum += m_db.Matrix().interpWarm[sp][ws](angle) / ( m_sigma_z[sp] *
                    m_windSpeedsAtHeightWarm.w[sp][mr] ) * m_depletionsWarm[sp] *
                    std::exp( - (m_sourceHeight * m_sourceHeight) / (2 * m_sigma_z[sp] *
                    m_sigma_z[sp]) );
            }
        }
    }

    return true;
}

double DilutionsCalculator::CalculateAngle(const size_t x, const size_t y) const
{
    double deltaLon = m_db.Landscape().at(x, y).coord.lon - m_sourceCoord.lon;
    double deltaLat = m_db.Landscape().at(x, y).coord.lat - m_sourceCoord.lat;
    return DEG(std::atan2(deltaLat, deltaLon));
}

bool DilutionsCalculator::InitWindSpeedsCache()
{
    const double Hg = m_sourceHeight;

    for (size_t sp = mt::SP_A; sp < mt::SP_COUNT; ++sp)
    {
        for (size_t mr = mt::MR_SNOW; mr < mt::MR_COUNT; ++mr)
        {
            m_windSpeedsAtHeightCold.w[sp][mr] = m_db.Matrix().avWindSpBySPCold[sp] *
                    std::pow(Hg / mt::h_f, m_db.Epsilon().e[sp][mr]);
            m_windSpeedsAtHeightWarm.w[sp][mr] = m_db.Matrix().avWindSpBySPWarm[sp] *
                    std::pow(Hg / mt::h_f, m_db.Epsilon().e[sp][mr]);
        }
    }

    return true;
}

bool DilutionsCalculator::InitDecayRate()
{
    mt::t_nuclideName nuclide = m_db.Emissions().find(m_emissionId)->second.nuclideName;
    ///< halflife in years, calculate decay rate in sec^-1
    m_decayRate = std::log(2) / ( m_db.Nuclides().find(nuclide)->second.halflife * 3.154e+7);
    return true;
}
