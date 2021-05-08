#include "geography.h"

#include <cmath>
#include <cassert>

using namespace mt;

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
 * @return Distance beyween point 1 and point 2
 */
double calculate_distance(t_epsg4326coord p1, t_epsg4326coord p2)
{
    p1.SetUnits(t_epsg4326coord::RADIANS);
    p2.SetUnits(t_epsg4326coord::RADIANS);
    return haversine_formula(p1.lat, p2.lat, p1.lon, p2.lon, earth_rad);
}
