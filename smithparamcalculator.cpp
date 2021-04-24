#include "smithparamcalculator.h"
#include "database.h"
#include "datainterface.h"
#include "sourcesloader.h"

#include <cmath>

SmithParamCalculator::SmithParamCalculator(const mm::t_observation& obs) :
    BaseCalculator(),
    m_obs(obs),
    m_latitude(0),
    m_longitude(0)
{

}

BaseCalculator::t_errorCode SmithParamCalculator::Execute()
{
    switch (m_db.Sources().at(0).coordinatesType)
    {
    case SourcesLoader::EPSG3857:
        m_latitude = 1;
        m_longitude = 1;
        break;
    case SourcesLoader::RELATIVE:
        MY_LOG("program not ready yet to handle relative coordinates");
        return NOT_SUPPORTED;
    }

    CalcSunDeclination();
    CalcSunAngle();
    CalcSetOfDay();
    CalcInsolClass();

    return OK;
}

mm::t_smithParam SmithParamCalculator::Get() const
{
    return m_param;
}

void SmithParamCalculator::CalcSunAngle()
{
    double hourAngle = M_PI * ( static_cast<double>(m_obs.dateTime.time().hour()) / 12.0 - 1 );
    double sunAngleSin = sin(m_sunDecl) * sin(RAD(m_latitude)) +
                         cos(m_sunDecl) * cos(RAD(m_latitude)) *
                         cos(hourAngle);
    m_sunAngle = 180 / M_PI * asin(sunAngleSin);
}

void SmithParamCalculator::CalcSetOfDay()
{

}

void SmithParamCalculator::CalcSunDeclination()
{
    int dayNumber = 30 * (m_obs.dateTime.date().month() - 1) + m_obs.dateTime.date().day();
    double sunLongitude = 4.909 + 1.705e-2 * dayNumber;
    m_sunDecl = asin(0.39795 * sin(sunLongitude));
}

void SmithParamCalculator::CalcInsolClass()
{

}
