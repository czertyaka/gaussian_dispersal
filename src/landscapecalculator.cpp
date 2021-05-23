#include "landscapecalculator.h"
#include "database.h"
#include "datainterface.h"
#include "geography.h"

#include <cstdlib>
#include <cmath>
#include <stdexcept>
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
        const mt::t_source& src = iter->second;
        if (src.coordinates.lat < *m_db.CoordSet().lat.begin()
                || src.coordinates.lat > *(--m_db.CoordSet().lat.end())
                || src.coordinates.lon < *m_db.CoordSet().lon.begin()
                || src.coordinates.lon > *(--m_db.CoordSet().lon.end()))
        {
                MY_LOG("source with coordinates " << src.coordinates << " is out of"
                       " area covered with geospatial data");
                return ERROR;
        }

        // allocate new array of distance mask for current source
        mt::t_distances distances;
        distances.value.Init(m_db.Landscape());
        distances.mask.Init(m_db.Landscape());

        // calculate distances
        CalculateDistances(iter->first, distances);
        m_db.Distances().insert(std::make_pair(iter->first, distances));

        // allocate new array of terrain corrections for current source coordinates
        mt::t_terrainCorrections corrs;
        corrs.Init(m_db.Landscape());

        // fill it
        if (!CalculateCorrections(corrs, src))
        {
            MY_LOG("failed to calulate terrain corrections for source with coordinates "
                    << src.coordinates);
            return ERROR;
        }

        // push back to vector in database
        m_db.TerrainCorrections().insert(std::make_pair(iter->first, corrs));
    }

    return OK;
}

void LandscapeCalculator::CalculateDistances(const size_t srcId, mt::t_distances& distances)
{
    for (size_t y = 0; y < m_yDim; ++y)
    {
        for (size_t x = 0; x < m_xDim; ++x)
        {
            const mt::t_point& point = m_db.Landscape().at(x, y);
            double distance = calculate_distance(m_db.Sources().find(srcId)->second.coordinates, point.coord);

            distances.value.at(x, y) = distance;
            distances.mask.at(x, y) = distance < gaussian_model_limit;
        }
    }
}

bool LandscapeCalculator::CalculateCorrections(mt::t_terrainCorrections &corrs, const mt::t_source &source)
{ 
    for (size_t y = 0; y < m_yDim; ++y)
    {
        for (size_t x = 0; x < m_xDim; ++x)
        {
            // calculate slope
            double slope = CalcSlope(x, y, source);

            // check if successful
            if (m_error)
            {
                return false;
            }

            corrs.at(x, y) = CalcCorrection(slope);

            // and then again
            if (m_error)
            {
                return false;
            }
        }
    }

    return true;
}

double LandscapeCalculator::CalcSlope(const double x, const double y, const mt::t_source &source) const
{
    mt::t_point point(m_db.Landscape().at(x, y));
    if (point.coord == source.coordinates)
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
    double lat = point.coord.lat;
    double lon = point.coord.lon;

    mt::t_point prevPoint;

    // get quarter encoding
    t_quarter quarter = static_cast<t_quarter>((lon < lon0) << 1 | (lat < lat0));

    // get tan of the angle
    double tana = (lon0 - lon) ? static_cast<double>(std::abs(lat0 - lat))
                    / static_cast<double>(std::abs(lon0 - lon))
                    : std::numeric_limits<double>::max();

    // all the cases...
    try
    {
        if (tana < std::tan(M_PI/8))
        {
            switch (quarter)
            {
            case I:
            case IV:
                prevPoint = m_db.Landscape().at(x + 1, y);
                break;
            default:
                prevPoint = m_db.Landscape().at(x - 1, y);
                break;
            }
        }
        else if (tana < std::tan(3*M_PI/8))
        {
            switch (quarter)
            {
            case I:
                prevPoint = m_db.Landscape().at(x + 1, y + 1);
                break;
            case II:
                prevPoint = m_db.Landscape().at(x - 1, y + 1);
                break;
            case III:
                prevPoint = m_db.Landscape().at(x - 1, y - 1);
                break;
            default:
                prevPoint = m_db.Landscape().at(x + 1, y - 1);
                break;
            }
        }
        else
        {
            switch (quarter)
            {
            case I:
            case II:
                prevPoint = m_db.Landscape().at(x, y + 1);
                break;
            default:
                prevPoint = m_db.Landscape().at(x, y - 1);
                break;
            }
        }
    }
    catch(const std::out_of_range& ex) //TODO
    {
        MY_LOG("warning: failed to estimate second point to calculate slope for " << point.coord);
        MY_LOG(ex.what());
        m_error = ERROR;
        return 0;
    }

    // calculate slope value
    t_epsg3857coord pointCoord = t_epsg3857coord(point.coord);
    t_epsg3857coord prevCoord = t_epsg3857coord(prevPoint.coord);

    double slope = static_cast<double>(point.elevation - prevPoint.elevation) /
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
