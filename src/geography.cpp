/*
 * based on https://www.iogp.org/wp-content/uploads/2019/09/373-07-02.pdf
 *
 * IOGP Publication 373-7-2 – Geomatics Guidance Note number 7, part 2 – September 2019
 * Coordinate Conversions and Transformations including Formulas
 * p. 44
*/

#include "geography.h"
#include "globaltypes.h"

#include <cmath>
#include <cassert>

using namespace mt;

// WGS 84 / Pseudo-Mercator (EPSG CRS code 3857) Parameters
static const double a = 6378137.0; ///<  ellipsoid semi-major axis, meters
//static const double lat_o = 0; ///< Latitude of natural origin, rad;  not in use
static const double lon_o = 0; ///< Longitude of natural origin, rad
static const double FE = 0; ///< False easting, meters
static const double FN = 0; ///< False northing, meters

epsg3857coord::epsg3857coord(double easting, double northing) :
    easting(easting),
    northing(northing)
{

}

epsg3857coord::epsg3857coord(const epsg4326coord &o)
{
    easting = FE + a * (RAD(o.lon) - lon_o);
    northing = FN + a * log(tan(M_PI/4 + RAD(o.lat)/2));
}

bool epsg3857coord::operator==(const epsg3857coord &o) const
{
    return easting == o.easting && northing == o.northing;
}

epsg4326coord::epsg4326coord(double lon, double lat, t_Unit unit) :
    lon(lon),
    lat(lat),
    m_unit(unit)
{

}

epsg4326coord::epsg4326coord(const epsg3857coord &o)
{
    double D = (FN - o.northing)/a;
    lat = DEG(M_PI/2 - 2*atan(exp(D)));
    lon = DEG((o.easting - FE)/a + lon_o);
}

bool epsg4326coord::operator==(const epsg4326coord &o) const
{
    return lon == o.lon && lat == o.lat;
}

void epsg4326coord::SetUnits(epsg4326coord::t_Unit unit)
{
    if (unit != m_unit)
    {
        switch (unit)
        {
        case DEGREES:
            lon = DEG(lon);
            lat = DEG(lat);
            break;
        case RADIANS:
            lon = RAD(lon);
            lat = RAD(lat);
        }
    }
}

QTextStream& operator<<(QTextStream &os, const epsg4326coord &cd)
{
    os << "lat = " << cd.lat << ", lon = " << cd.lon;
    return os;
}

static const double earth_rad = 6371000; ///< earth radius in meters

/**
 * @brief haversine_formula Implements haversine formula for distance between
 *        two points on shpere
 * @details https://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1 Latitude of point 1, radians
 * @param lat2 Latitude of point 2, radians
 * @param lon1 Longitude of point 1, radians
 * @param lon2 Longitude of point 2, radians
 * @param rad Radius of sphere, meters
 * @return Distance beyween point 1 and point 2
 */
static inline double haversine_formula(const double lat1, const double lat2,
                                       const double lon1, const double lon2,
                                       const double rad)
{
    using namespace std;

    double h = pow( sin((lat2 - lat1) / 2), 2 ) + cos(lat1) * cos(lat2)
                * pow( sin((lon2 - lon1) / 2), 2 );
    assert(h >= 0 && h < 1);
    return 2 * rad * asin(sqrt(h));
}

/**
 * @brief calculate_distance Uses habersine formula for sphere to
 *        calculate distance between two points on earth
 * @param p1 Firts point coordinates
 * @param p2 Second point coordinates
 * @return Distance between point 1 and point 2, meters
 */
double calculate_distance(t_epsg4326coord p1, t_epsg4326coord p2)
{
    p1.SetUnits(t_epsg4326coord::RADIANS);
    p2.SetUnits(t_epsg4326coord::RADIANS);
    return haversine_formula(p1.lat, p2.lat, p1.lon, p2.lon, earth_rad);
}
