#include "database.h"
#include "globaltypes.h"
#include "geography.h"

using namespace mt;

Matrix::Matrix()
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

const std::optional<t_quarterEmissionValue>& emissionValue::getQuarter() const
{
    return m_quarterValue;
}

void Source::InitCoordinates()
{
    switch (m_coordType)
    {
    case EPSG4326:
        coordinates = t_epsg4326coord(m_rawX, m_rawY);
        break;
    case EPSG3857:
        coordinates = t_epsg4326coord(t_epsg4326coord(m_rawX, m_rawY));
        break;
    case RELATIVE:
        assert(m_rawX <= 100);
        assert(m_rawY <= 100);
#ifndef TESTING
        const DataBase::t_coordSet& set = DataBase::GetInstance().CoordSet();
        assert(set.lat.size() && set.lon.size());
        double lon = m_rawX / 100 * (*(--set.lon.end()) - *set.lon.begin()) + *set.lon.begin();
        double lat = m_rawY / 100 * (*(--set.lat.end()) - *set.lat.begin()) + *set.lat.begin();
#else
        double lon = 0;
        double lat = 0;
#endif
        coordinates = t_epsg4326coord(lon, lat);
        break;
    }
}

void Source::SetRawCoordinates(double x, double y)
{
    m_rawX = x;
    m_rawY = y;
}

void Source::SetType(Source::t_coordType type)
{
    m_coordType = type;
}

InterpolatedSlice::InterpolatedSlice()
{
    for (size_t n = 0; n < sizeof(m_coeffs)/sizeof(m_coeffs[0]); ++n)
    {
        m_coeffs[n].a = 0;
        m_coeffs[n].b = 0;
        m_coeffs[n].c = 0;
        m_coeffs[n].d = 0;
    }
}

static double round_degrees(double degree)
{
    // get remainer
    degree = std::fmod(degree, 360);

    // make it positive
    degree = degree < 0 ? degree + 360 : degree;

    return degree;
}

double InterpolatedSlice::operator()(double degree) const
{
    degree = round_degrees(degree);

    // get direction
    mt::t_windDir windDir = mt::degree_to_dir(degree);

    // make degrees [0, 22.5]
    if (degree < 11.25)
    {
        degree += 11.25;
    }
    else if (degree > 348.75)
    {
        degree -= 348.75;
    }
    else
    {
        degree -= t_matrix::windDirVals[windDir] - 11.25;
    }

    // calc x [0, 1]
    double x = degree / 22.5;

    // calculate
    return std::pow(x, 3) * m_coeffs[windDir].a + std::pow(x, 2) * m_coeffs[windDir].b +
            x * m_coeffs[windDir].c + m_coeffs[windDir].d;
}

void InterpolatedSlice::Init(t_nodes& nodes)
{
    for (int i = 0; i < static_cast<int>(sizeof(m_coeffs)/sizeof(m_coeffs[0])); ++i)
    {
        double p0 = i - 1 >= 0 ? nodes[i - 1] : nodes[WIND_DIR_COUNT - 1];
        double p1 = nodes[i];
        double p2 = nodes[(i + 1) % WIND_DIR_COUNT];
        double p3 = nodes[(i + 2) % WIND_DIR_COUNT];

        m_coeffs[i].a = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
        m_coeffs[i].b = p0 - 2.5 * p1 + 2 * p2 - 0.5 * p3;
        m_coeffs[i].c = -0.5 * p0 + 0.5 * p2;
        m_coeffs[i].d = p1;
    }
}

/**
 * @brief degree_to_dir Converts degrees to wind directions
 * @param degree Degrees in ° with 0° = east direction
 * @return wind directions
 */
mt::t_windDir mt::degree_to_dir(double degree)
{    
    degree = round_degrees(degree);

    // shift polar coordiantes so 11.25° = north
    degree  = degree > 78.75 ? degree - 78.75 : degree + 3 * 90 + 11.25;

    std::array<double, WIND_DIR_COUNT> windDirsDegrees = {
        22.5,
        45,
        67.5,
        90,
        112.5,
        135,
        157.5,
        180,
        202.5,
        225,
        247.5,
        270,
        292.5,
        315,
        337.5,
        360
    };

    return static_cast<t_windDir>(std::distance(windDirsDegrees.begin(),
           std::lower_bound(windDirsDegrees.begin(), windDirsDegrees.end(), degree)));
}
