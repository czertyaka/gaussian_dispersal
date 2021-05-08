/*
 * based on https://www.iogp.org/wp-content/uploads/2019/09/373-07-02.pdf
 *
 * IOGP Publication 373-7-2 – Geomatics Guidance Note number 7, part 2 – September 2019
 * Coordinate Conversions and Transformations including Formulas
 * p. 44
*/

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
