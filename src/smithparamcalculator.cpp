#include "smithparamcalculator.h"
#include "database.h"
#include "datainterface.h"
#include "sourcesloader.h"

#include <cmath>

SmithParamCalculator::SmithParamCalculator(const mm::t_observation& obs) :
    BaseCalculator(),
    m_obs(obs),
    m_coord(mm::t_epsg4326coord())
{

}

BaseCalculator::t_errorCode SmithParamCalculator::Execute()
{
    if (!GetCoordinate())
    {
        MY_LOG("coordinate can not be estimated");
        return ERROR;
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

/**
 * @brief SmithParamCalculator::GetCoordinate Search for any coordinates in database
 * @return true if found
 */
bool SmithParamCalculator::GetCoordinate()
{
    if (!m_db.Landscape().empty())
    {
        m_coord = m_db.Landscape().at(0).coord;
    }
    else if (m_db.Image().borders.has_value())
    {
        m_coord = m_db.Image().borders.value().nw;
    }
    else if (m_db.Sources().at(0).coordinatesType != SourcesLoader::RELATIVE)
    {
        SourcesLoader::t_source& source = m_db.Sources().at(0);

        switch (source.coordinatesType)
        {
        case SourcesLoader::EPSG4326:
            m_coord = mm::t_epsg4326coord(source.x, source.y);
            break;
        case SourcesLoader::EPSG3857:
            m_coord = mm::t_epsg4326coord(mm::t_epsg3857coord(source.x, source.y));
            break;
        default:
            break;
        }
    }
    else
    {
        return false;
    }

    return true;
}

void SmithParamCalculator::CalcSunAngle()
{
    double hourAngle = M_PI * ( static_cast<double>(m_obs.dateTime.time().hour()) / 12.0 - 1 );
    double sunAngleSin = sin(m_sunDecl) * sin(m_coord.lat) +
                         cos(m_sunDecl) * cos(m_coord.lon) *
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
