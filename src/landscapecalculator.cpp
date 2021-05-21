#include "landscapecalculator.h"
#include "database.h"
#include "datainterface.h"
#include "geography.h"

#include <cstdlib>
#include <cmath>
#include <limits>

LandscapeCalculator::LandscapeCalculator() :
    BaseCalculator(),
    m_xDim(m_db.CoordSet().lon.size()),
    m_yDim(m_db.CoordSet().lat.size())
{

}

LandscapeCalculator::~LandscapeCalculator()
{

}

BaseCalculator::t_errorCode LandscapeCalculator::Execute()
{
    m_xDim = m_db.CoordSet().lon.size();
    m_yDim = m_db.CoordSet().lat.size();

    m_db.TerrainCorrections().clear();
    m_db.Distances().clear();

    // calculate array for each source coordinate
    for (auto iter = m_db.Sources().cbegin(); iter != m_db.Sources().cend(); ++iter)
    {
        // check if given source is within landscape data
        // TODO: move earlier in calculation timeline
        if (iter->coordinates.lat < *m_db.CoordSet().lat.begin()
                || iter->coordinates.lat > *(--m_db.CoordSet().lat.end())
                || iter->coordinates.lon < *m_db.CoordSet().lon.begin()
                || iter->coordinates.lon > *(--m_db.CoordSet().lon.end()))
        {
                MY_LOG("source with coordinates " << iter->coordinates << " is out of"
                       " area covered with geospatial data");
                return ERROR;
        }

        // allocate new array of distance mask for current source
        dbt::t_distanceMask distanceMask(*iter);
        distanceMask.distances.resize(m_db.Landscape().size());
        distanceMask.mask.resize(m_db.Landscape().size());

        // calculate distances
        CalculateDistances(distanceMask);
        m_db.Distances().push_back(distanceMask);


        // allocate new array of terrain corrections for current source coordinates
        dbt::t_srcTerrainCorrections corrs(*iter);
        corrs.data.resize(m_db.Landscape().size());

        // fill it
        if (!CalculateCorrections(corrs, *iter))
        {
            MY_LOG("failed to calulate terrain corrections for source with coordinates "
                    << iter->coordinates);
            return ERROR;
        }

        // push back to vector in database
        m_db.TerrainCorrections().push_back(corrs);
    }

    return OK;
}

void LandscapeCalculator::CalculateDistances(dbt::t_distanceMask& distanceMask)
{
    for (size_t y = 0; y < m_yDim; ++y)
    {
        for (size_t x = 0; x < m_xDim; ++x)
        {
            dbt::t_landscape::const_iterator pointIt = m_db.Landscape().begin() + y * m_xDim + x;
            double distance = calculate_distance(distanceMask.source.coordinates, pointIt->coord);

            distanceMask.distances.at(y * m_xDim + x) = distance;
            distanceMask.mask.at(y * m_xDim + x) = distance < gaussian_model_limit;
        }
    }
}

bool LandscapeCalculator::CalculateCorrections(dbt::t_srcTerrainCorrections &corrs, const mt::t_source &source)
{ 
    // we will treat vector as 2-D array from now on
    // the values in landscape vector stored in [lat][lon] order
    // let's loop over every point and get it done
    for (size_t y = 0; y < m_yDim; ++y)
    {
        for (size_t x = 0; x < m_xDim; ++x)
        {
            dbt::t_landscape::const_iterator point = m_db.Landscape().begin() + y * m_xDim + x;

            // calculate slope
            double slope = CalcSlope(point, source);

            // check if successful
            if (m_error)
            {
                return false;
            }

            corrs.data.at(y * m_xDim + x) = CalcCorrection(slope);

            // and then again
            if (m_error)
            {
                return false;
            }
        }
    }

    return true;
}

double LandscapeCalculator::CalcSlope(const dbt::t_landscape::const_iterator point, const mt::t_source &source) const
{
    if (point->coord == source.coordinates)
    {
        return 0;
    }

    enum t_quarter : uint8_t
    {
        I   = 0b11,
        II  = 0b01,
        III = 0b00,
        IV  = 0b10
    };

    double lat0 = source.coordinates.lat;
    double lon0 = source.coordinates.lon;
    double lat = point->coord.lat;
    double lon = point->coord.lon;

    dbt::t_landscape::const_iterator prevPoint;

    // get quarter encoding
    t_quarter quarter = static_cast<t_quarter>((lon < lon0) << 1 | (lat < lat0));

    // get tan of the angle
    double tana = (lon0 - lon) ? static_cast<double>(std::abs(lat0 - lat))
                    / static_cast<double>(std::abs(lon0 - lon))
                    : std::numeric_limits<double>::max();

    // all the cases...
    if (tana < std::tan(M_PI/8))
    {
        switch (quarter)
        {
        case I:
        case IV:
            prevPoint = point + 1;
            break;
        default:
            prevPoint = point - 1;
            break;
        }
    }
    else if (tana < std::tan(3*M_PI/8))
    {
        switch (quarter)
        {
        case I:
            prevPoint = point + m_xDim + 1;
            break;
        case II:
            prevPoint = point + m_xDim - 1;
            break;
        case III:
            prevPoint = point - m_xDim - 1;
            break;
        default:
            prevPoint = point - m_xDim + 1;
            break;
        }
    }
    else
    {
        switch (quarter)
        {
        case I:
        case II:
            prevPoint = point + m_xDim;
            break;
        default:
            prevPoint = point - m_xDim;
            break;
        }
    }

    // just in case...
    if (!(prevPoint >= m_db.Landscape().cbegin() && prevPoint < m_db.Landscape().cend()))
    {
        MY_LOG("warning: failed to estimate second point to calculate slope for " << point->coord);
        m_error = ERROR;
        return 0;
    }

    // calculate slope value
    t_epsg3857coord pointCoord = t_epsg3857coord(point->coord);
    t_epsg3857coord prevCoord = t_epsg3857coord(prevPoint->coord);

    double slope = static_cast<double>(point->elevation - prevPoint->elevation) /
                    std::sqrt( std::pow(pointCoord.easting - prevCoord.easting, 2) +
                               std::pow(pointCoord.northing - prevCoord.northing, 2));

    return slope;
}

double LandscapeCalculator::CalcCorrection(const double slope) const
{
    double correction;
    double localSlope = slope;

    if (std::abs(localSlope) > 0.45)
    {
        MY_LOG("warning: calculated slope absolute value |" << localSlope
               << "| exceeds 0.45");
        localSlope = localSlope > 0 ? 0.45 : -0.45;
    }

    if (localSlope < -0.05)
    {
        correction = 1 + 2.15 * localSlope + 1.5 * localSlope * localSlope;
    }
    else if (localSlope < 0.05)
    {
        correction = 1;
    }
    else
    {
        correction = 1 + 0.6 * localSlope + 4 * localSlope * localSlope;
    }

    return correction;
}
