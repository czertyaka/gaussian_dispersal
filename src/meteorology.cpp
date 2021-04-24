/*
 * based on https://www.iogp.org/wp-content/uploads/2019/09/373-07-02.pdf
 *
 * IOGP Publication 373-7-2 – Geomatics Guidance Note number 7, part 2 – September 2019
 * Coordinate Conversions and Transformations including Formulas
 * p. 44
*/

#include "meteorology.h"

using namespace mm;

// WGS 84 / Pseudo-Mercator (EPSG CRS code 3857) Parameters
static const double a = 6378137.0; ///<  ellipsoid semi-major axis, meters
//static const double lat_o = 0; ///< Latitude of natural origin, rad;  not in use
static const double lon_o = 0; ///< Longitude of natural origin, rad
static const double FE = 0; ///< False easting, meters
static const double FN = 0; ///< False northing, meters

epsg3857coord::epsg3857coord(const mm::epsg4326coord &o)
{
    easting = FE + a * (o.lon - lon_o);
    northing = FN + a * log(tan(M_PI/4 + o.lat/2));
}

epsg4326coord::epsg4326coord(const mm::epsg3857coord &o)
{
    double D = (FN - o.northing)/a;
    lat = M_PI/2 - 2*atan(exp(D));
    lon = (o.easting - FE)/a + lon_o;
}
