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
    double commonFactor = 2. * m_matrix.N / std::pow(2 * M_PI, 1.5);
    double coldFraction = static_cast<double>(m_matrix.MCold) / (m_matrix.MCold + m_matrix.MWarm);
    double warmFraction = 1 - coldFraction;

    mt::t_terrainCorrections& corrs = m_db.TerrainCorrections().find(m_sourceId)->second;
    mt::t_distances& distances = m_db.Distances().find(m_sourceId)->second;

    for (size_t x = 0; x < m_xDim; ++x)
    {
        for (size_t y = 0; y < m_yDim; ++y)
        {
            // skip if we're far too beyond gaussioan model
            if (!distances.mask.at(x, y))
            {
                continue;
            }

            // / ( Rn * x ) from formula
            commonFactor /= ( corrs.at(x, y) * distances.value.at(x, y) );
        }
    }

    return true;
}
