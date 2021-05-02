/*
 * based on https://www.iogp.org/wp-content/uploads/2019/09/373-07-02.pdf
 *
 * IOGP Publication 373-7-2 – Geomatics Guidance Note number 7, part 2 – September 2019
 * Coordinate Conversions and Transformations including Formulas
 * p. 44
*/

#include "globaltypes.h"

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

epsg3857coord::epsg3857coord(const mt::epsg4326coord &o)
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

epsg4326coord::epsg4326coord(const mt::epsg3857coord &o)
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

matrix::matrix()
{
    for (size_t n = 0; n < N; ++n)
    {
        for (size_t k = 0; k < K; ++k)
        {
            windSpRepByCPCold[n][k] = 0;
            windSpRepByCPWarm[n][k] = 0;
            for (size_t j = 0; j < J; ++j)
            {
                mCold[n][j][k] = 0;
                mWarm[n][j][k] = 0;
                wCold[n][j][k] = 0;
                wWarm[n][j][k] = 0;
            }
        }
    }
}

emissionValue::emissionValue()
{
    m_annualValue.reset();
    m_quarterValue.reset();
}

emissionValue::emissionValue(double annualValue) :
    m_annualValue(annualValue)
{
    m_quarterValue.reset();
}

emissionValue::emissionValue(double first, double second, double third, double fourth)
{
    m_annualValue.reset();
    m_quarterValue->fisrtQuarter = first;
    m_quarterValue->secondQuarter = second;
    m_quarterValue->thirdQuarter = third;
    m_quarterValue->fourthQuarter = fourth;
}

emissionValue &emissionValue::operator=(const emissionValue &o)
{
    m_annualValue = o.m_annualValue;
    m_quarterValue = o.m_quarterValue;
    return *this;
}

const std::optional<double> &emissionValue::getAnnual() const
{
    return m_annualValue;
}

const std::optional<t_quarterEmission>& emissionValue::getQuarter() const
{
    return m_quarterValue;
}

bool source::operator==(const source &o) const
{
    return coordinates == o.coordinates && height == o.height;
}

bool nuclide::operator<(const nuclide &o) const
{
    return name < o.name;
}

bool nuclide::operator==(const nuclide &o) const
{
    return name == o.name;
}
