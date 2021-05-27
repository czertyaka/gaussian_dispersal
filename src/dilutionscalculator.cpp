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

        // update vector
        m_dilutions.clear();
        m_dilutions.resize(m_db.Landscape().size());
        std::fill(m_dilutions.begin(), m_dilutions.end(), 0);

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

    double commonFactor = 2. * m_matrix.N / std::pow(2 * M_PI, 1.5);
    double coldFraction = static_cast<double>(m_matrix.MCold) / (m_matrix.MCold + m_matrix.MWarm);
    double warmFraction = 1 - coldFraction;

    mt::t_terrainCorrections& corrs = m_db.TerrainCorrections().find(m_sourceId)->second;
    m_distances = &m_db.Distances().find(m_sourceId)->second;

    size_t x = 0;
    size_t y = 0;
    for (; x < m_xDim; ++x)
    {
        for (; y < m_yDim; ++y)
        {
            // skip if we're far too beyond gaussioan model
            if (!m_distances->mask.at(x, y))
            {
                continue;
            }

            // / ( Rn * x ) from formula
            commonFactor /= ( corrs.at(x, y) * m_distances->value.at(x, y) );

            // calculate diff parameter for current point
            if (!CalculateDiffusionParameter(x, y))
            {
                result &= false;
                break;
            }
        }
    }

    if (!result)
    {
        MY_LOG("failed to calculate dilution factor for " << m_db.Landscape().at(x, y).coord);
    }

    return result;
}

bool DilutionsCalculator::CalculateDiffusionParameter(const size_t x, const size_t y)
{
    m_sigma_z.fill(0);

    const mt::t_diffusionParameter& max = m_db.MaxDiffusionPatameters();

    // get current distance
    double r = m_distances->value.at(x, y);

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
