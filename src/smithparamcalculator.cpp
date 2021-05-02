#include "smithparamcalculator.h"
#include "database.h"
#include "datainterface.h"
#include "sourcesloader.h"

#include <cmath>

SmithParamCalculator::SmithParamCalculator(const mt::t_observation& obs) :
    BaseCalculator(),
    m_obs(obs),
    m_coord(mt::t_epsg4326coord())
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

    if      (m_obs.windSpeed >= 0 && m_obs.windSpeed < 1.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathG; break;
        case -2:   m_param = mt::cathF; break;
        case -1:   m_param = mt::cathF; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathC; break;
        case 2:    m_param = mt::cathB; break;
        case 3:    m_param = mt::cathA; break;
        case 4:    m_param = mt::cathA; break;
        case 5:    m_param = mt::cathA; break;
        default:    break;
        }
    }
    else if (m_obs.windSpeed >= 1.5 && m_obs.windSpeed < 2.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathG; break;
        case -2:   m_param = mt::cathF; break;
        case -1:   m_param = mt::cathE; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathC; break;
        case 2:    m_param = mt::cathB; break;
        case 3:    m_param = mt::cathB; break;
        case 4:    m_param = mt::cathA; break;
        case 5:    m_param = mt::cathA; break;
        default: break;
        }
    }
    else if (m_obs.windSpeed >= 2.5  && m_obs.windSpeed < 3.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathF; break;
        case -2:   m_param = mt::cathF; break;
        case -1:   m_param = mt::cathE; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathD; break;
        case 2:    m_param = mt::cathC; break;
        case 3:    m_param = mt::cathB; break;
        case 4:    m_param = mt::cathB; break;
        case 5:    m_param = mt::cathA; break;
        default: break;
        }
    }
    else if (m_obs.windSpeed >= 3.5  && m_obs.windSpeed < 4.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathF; break;
        case -2:   m_param = mt::cathE; break;
        case -1:   m_param = mt::cathD; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathD; break;
        case 2:    m_param = mt::cathC; break;
        case 3:    m_param = mt::cathB; break;
        case 4:    m_param = mt::cathB; break;
        case 5:    m_param = mt::cathA; break;
        default: break;
        }
    }
    else if (m_obs.windSpeed >= 4.5  && m_obs.windSpeed < 5.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathE; break;
        case -2:   m_param = mt::cathE; break;
        case -1:   m_param = mt::cathD; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathD; break;
        case 2:    m_param = mt::cathC; break;
        case 3:    m_param = mt::cathC; break;
        case 4:    m_param = mt::cathB; break;
        case 5:    m_param = mt::cathB; break;
        default: break;
        }
    }
    else if (m_obs.windSpeed >= 5.5  && m_obs.windSpeed < 6.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathE; break;
        case -2:   m_param = mt::cathD; break;
        case -1:   m_param = mt::cathD; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathD; break;
        case 2:    m_param = mt::cathC; break;
        case 3:    m_param = mt::cathC; break;
        case 4:    m_param = mt::cathC; break;
        case 5:    m_param = mt::cathB; break;
        default: break;
        }
    }
    else if (m_obs.windSpeed >= 6.5  && m_obs.windSpeed < 7.5)
    {
        switch (m_insolClass)
        {
        case -3:   m_param = mt::cathD; break;
        case -2:   m_param = mt::cathD; break;
        case -1:   m_param = mt::cathD; break;
        case 0:    m_param = mt::cathD; break;
        case 1:    m_param = mt::cathD; break;
        case 2:    m_param = mt::cathD; break;
        case 3:    m_param = mt::cathC; break;
        case 4:    m_param = mt::cathC; break;
        case 5:    m_param = mt::cathC; break;
        default: break;
        }
    }
    else if (m_obs.windSpeed >= 7.5)
    {
       m_param = mt::cathD;
    }

    return OK;
}

mt::t_smithParam SmithParamCalculator::Get() const
{
    return m_param;
}

/**
 * @brief SmithParamCalculator::GetCoordinate Search for any coordinates in database
 * @return true if found
 */
bool SmithParamCalculator::GetCoordinate()
{
    // get coordiantes from geosaptial data
    if (!m_db.Landscape().empty())
    {
        m_coord = m_db.Landscape().at(0).coord;
    }
    // if there is none, look for image borders
    else if (m_db.Image().borders.has_value())
    {
        m_coord = m_db.Image().borders.value().nw;
    }
    // at last, check fisrt source coordinate
    else if (!m_db.Sources().empty())
    {
        m_coord = m_db.Sources().at(0).coord;
    }
    else
    {
        return false;
    }

    m_coord.SetUnits(mt::t_epsg4326coord::RADIANS);

    return true;
}

void SmithParamCalculator::CalcSunDeclination()
{
    int dayNumber = 30 * (m_obs.dateTime.date().month() - 1) + m_obs.dateTime.date().day();
    double sunLongitude = 4.909 + 1.705e-2 * dayNumber;
    m_sunDecl = asin(0.39795 * sin(sunLongitude));
}

void SmithParamCalculator::CalcSunAngle()
{
    m_coord.SetUnits(mt::t_epsg4326coord::RADIANS);

    double hourAngle = M_PI * ( static_cast<double>(m_obs.dateTime.time().hour()) / 12.0 - 1 );
    double sunAngleSin = sin(m_sunDecl) * sin(m_coord.lat) +
                         cos(m_sunDecl) * cos(m_coord.lon) *
                         cos(hourAngle);
    m_sunAngle = DEG(asin(sunAngleSin));
}

void SmithParamCalculator::CalcSetOfDay()
{
    m_coord.SetUnits(mt::t_epsg4326coord::RADIANS);

    double beta = - ( 0.0145 + sin(m_sunDecl) * sin(m_coord.lat) ) /
                    ( cos(m_sunDecl) * cos(m_coord.lat) );
    if (beta > 1)
    {
        beta = 1;
    }
    else if (beta < -1)
    {
        beta = -1;
    }
    m_setTime = 12 * (1 + 1/M_PI * acos(beta));
    m_riseTime = 24 - m_setTime;
}

void SmithParamCalculator::CalcInsolClass()
{
    int hour = m_obs.dateTime.time().hour();

    if (hour > m_riseTime + 1 && hour < m_setTime - 1) // дневной индекс инсоляции
    {
        if      (m_sunAngle >= 0  && m_sunAngle < 15) { m_insolClass = 1; }
        else if (m_sunAngle >= 15 && m_sunAngle < 30) { m_insolClass = 2; }
        else if (m_sunAngle >= 30 && m_sunAngle < 45) { m_insolClass = 3; }
        else if (m_sunAngle >= 45 && m_sunAngle < 60) { m_insolClass = 4; }
        else if (m_sunAngle >= 60)                    { m_insolClass = 5; }
    }
    else if (hour < m_riseTime && hour > m_setTime) // ночной индекс инсоляции
    {
        double timeDiff = hour - m_setTime;
        if      (timeDiff >= 0 && timeDiff < 2) {m_insolClass = -1 ; }
        else if (timeDiff >= 2 && timeDiff < 7) {m_insolClass = -2 ; }
        else if (timeDiff >= 7)                 {m_insolClass = -3 ; }
    }
    else // в течение 1 ч до захода и после восхода солнца
    {
        double r = CalcAlbedo();
        double N = 0.1 * m_obs.cloudAmount;

        if ( (1 - r) * (990 * sin(RAD(m_sunAngle)) - 30) * (1 - 0.75 * pow(N, 3.4)) >
             110 - 60 * N )
        {
            m_insolClass = 0;
        }
        else
        {
            m_insolClass = -1;
        }
    }

    CloudCorrection();
    FogCorrection();
    SnowCorrection();
}

double SmithParamCalculator::CalcAlbedo()
{
    m_coord.SetUnits(mt::t_epsg4326coord::DEGREES);

    if (m_coord.lat <= 90 && m_coord.lat > 70) { return 0.80; }

    switch (m_obs.dateTime.date().month())
    {
    case mt::JAN: // зима
    case mt::FEB:
    case mt::DEC:
        if (m_coord.lat <= 70 && m_coord.lat > 60) { return 0.70; }
        if (m_coord.lat <= 60 && m_coord.lat > 50) { return 0.60; }
        if (m_coord.lat <= 50 && m_coord.lat > 40) { return 0.49; }
        if (m_coord.lat <= 40 && m_coord.lat > 30) { return 0.34; }
        if (m_coord.lat <= 30 && m_coord.lat > 20) { return 0.26; }
        if (m_coord.lat <= 20 && m_coord.lat > 10) { return 0.24; }
        if (m_coord.lat <= 10 && m_coord.lat >= 0) { return 0.21; }
        break;
    case mt::MAR: // весна
    case mt::APR:
    case mt::MAY:
        if (m_coord.lat <= 70 && m_coord.lat > 60) { return 0.70; }
        if (m_coord.lat <= 60 && m_coord.lat > 50) { return 0.57; }
        if (m_coord.lat <= 50 && m_coord.lat > 40) { return 0.42; }
        if (m_coord.lat <= 40 && m_coord.lat > 30) { return 0.28; }
        if (m_coord.lat <= 30 && m_coord.lat > 20) { return 0.26; }
        if (m_coord.lat <= 20 && m_coord.lat > 10) { return 0.24; }
        if (m_coord.lat <= 10 && m_coord.lat >= 0) { return 0.21; }
        break;
    case mt::JUN: // лето
    case mt::JUL:
    case mt::AUG:
        if (m_coord.lat <= 70 && m_coord.lat > 60) { return 0.26; }
        if (m_coord.lat <= 60 && m_coord.lat > 50) { return 0.18; }
        if (m_coord.lat <= 50 && m_coord.lat > 40) { return 0.20; }
        if (m_coord.lat <= 40 && m_coord.lat > 30) { return 0.22; }
        if (m_coord.lat <= 30 && m_coord.lat > 20) { return 0.24; }
        if (m_coord.lat <= 20 && m_coord.lat > 10) { return 0.22; }
        if (m_coord.lat <= 10 && m_coord.lat >= 0) { return 0.19; }
        break;
    case mt::SEP: // осень
    case mt::OCT:
    case mt::NOV:
        if (m_coord.lat <= 70 && m_coord.lat > 60) { return 0.24; }
        if (m_coord.lat <= 60 && m_coord.lat > 50) { return 0.18; }
        if (m_coord.lat <= 50 && m_coord.lat > 40) { return 0.20; }
        if (m_coord.lat <= 40 && m_coord.lat > 30) { return 0.22; }
        if (m_coord.lat <= 30 && m_coord.lat > 20) { return 0.24; }
        if (m_coord.lat <= 20 && m_coord.lat > 10) { return 0.22; }
        if (m_coord.lat <= 10 && m_coord.lat >= 0) { return 0.19; }
        break;
    default:
        break;
    }

    return 0;
}

void SmithParamCalculator::CloudCorrection()
{
    bool dayTime = m_obs.dateTime.time().hour() > m_riseTime
            && m_obs.dateTime.time().hour() < m_setTime;

    switch (m_obs.cloudAmount)
    {

    case 0:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 1:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            CorrectionI();
            break;
        case 1:
            CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 2:
    case 3:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            CorrectionI();
            break;
        case 1:
            CorrectionI();
            break;
        case 2:
        case 3:
            CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 4:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            CorrectionI();
            break;
        case 1:
            CorrectionI();
            break;
        case 2:
        case 3:
            CorrectionI();
            break;
        case 4:
            CorrectionI();
            break;
        default:
            break;
        }

        break;

    case 5:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 1:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 4:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 5:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        default:
            break;
        }

        break;

    case 6:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 1:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 4:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 5:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 6:
            CorrectionII();
            break;
        default:
            break;
        }

        break;

    case 7:
    case 8:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 1:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 4:
            CorrectionII();
            break;
        case 5:
            CorrectionII();
            break;
        case 6:
            CorrectionII();
            break;
        case 7:
        case 8:
            CorrectionIV();
            break;
        default:
            break;
        }

        break;

    case 9:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 1:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 2:
        case 3:
            if (dayTime) { CorrectionI(); } else { CorrectionII(); }
            break;
        case 4:
            if (dayTime) { CorrectionII(); } else { CorrectionIII(); }
            break;
        case 5:
            if (dayTime) { CorrectionII(); } else { CorrectionIII(); }
            break;
        case 6:
            CorrectionIV();
            break;
        case 7:
        case 8:
            CorrectionIV();
            break;
        case 9:
            CorrectionIV();
            break;
        default:
            break;
        }

        break;

    case 10:

        switch (m_obs.lowerCloudAmount)
        {
        case 0:
            CorrectionIII();
            break;
        case 1:
            CorrectionIII();
            break;
        case 2:
        case 3:
            CorrectionIII();
            break;
        case 4:
            CorrectionIII();
            break;
        case 5:
            CorrectionIV();
            break;
        case 6:
            CorrectionIV();
            break;
        case 7:
        case 8:
            CorrectionIV();
            break;
        case 9:
            CorrectionV();
            break;
        case 10:
            CorrectionV();
            break;
        default:
            break;
        }

        break;

    default:
        break;
    }
}

void SmithParamCalculator::FogCorrection()
{
    if (m_obs.fog) { CorrectionVI(); }
}

void SmithParamCalculator::SnowCorrection()
{
    if (m_obs.snow) { CorrectionVII(); }
}

void SmithParamCalculator::CorrectionI()
{

}

void SmithParamCalculator::CorrectionII()
{
    switch (m_insolClass)
    {
    case -3:
    case -2:
        m_insolClass++;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        m_insolClass--;
        break;
    default:
        break;
    }
}

void SmithParamCalculator::CorrectionIII()
{
    switch (m_insolClass)
    {
    case -3:
    case -2:
        m_insolClass = -1;
        break;
    case 2:
        m_insolClass = 1;
        break;
    case 3:
    case 4:
    case 5:
        m_insolClass--;
        break;
    default:
        break;
    }
}

void SmithParamCalculator::CorrectionIV()
{
    switch (m_insolClass)
    {
    case -3:
    case -2:
        m_insolClass = -1;
        break;
    case 2:
        m_insolClass = 1;
        break;
    case 3:
    case 4:
    case 5:
        m_insolClass -= 2;
        break;
    default:
        break;
    }
}

void SmithParamCalculator::CorrectionV()
{
    switch (m_insolClass)
    {
    case -3:
    case -2:
    case -1:
    case 1:
        m_insolClass = 0;
        break;
    case 2:
    case 3:
    case 4:
        m_insolClass = 1;
        break;
    case 5:
        m_insolClass = 2;
        break;
    default:
        break;
    }
}

void SmithParamCalculator::CorrectionVI()
{
    m_insolClass = 0;
}

void SmithParamCalculator::CorrectionVII()
{
    switch (m_insolClass)
    {
    case -1:
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        m_insolClass--;
        break;
    case 5:
        m_insolClass = 3;
        break;
    default:
        break;
    }
}
