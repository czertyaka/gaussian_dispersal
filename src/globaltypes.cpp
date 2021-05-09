#include "database.h"
#include "globaltypes.h"
#include "geography.h"

using namespace mt;

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

bool Source::operator==(const Source &o) const
{
    return coordinates == o.coordinates && height == o.height;
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

bool nuclide::operator<(const nuclide &o) const
{
    return name < o.name;
}

bool nuclide::operator==(const nuclide &o) const
{
    return name == o.name;
}
