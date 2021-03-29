#include "rp5csvparser.h"
#include "datainterface.h"

#include <QStringList>
#include <QDebug>
#include <QRegExp>
#include <QDate>
#include <QTime>

Rp5CsvParser::Rp5CsvParser() :
    CsvParser(29, "Местное время"),
    m_prevE1(""),
    m_prevE2("")
{

}

CsvParser::t_lineStatus Rp5CsvParser::Parse(const QString &string, mm::t_observation &observation)
{   
    m_obs = &observation;

    t_lineStatus status = CsvParser::ParseLine(string);
    if (status != OK)
    {
        return status;
    }

    // read
    m_Time = m_list.at(0);
    m_T = m_list.at(1);
    m_DD = m_list.at(6);
    m_Ff = m_list.at(7);
    m_N = m_list.at(10);
    m_Nh = m_list.at(17);
    m_VV = m_list.at(21);
    m_E1 = m_list.at(25);
    m_E2 = m_list.at(27);

    // restore ground surface
    m_E1 = m_E1.isEmpty() ? m_prevE1 : m_E1;
    m_prevE1 = m_E1;
    m_E2 = m_E2.isEmpty() ? m_prevE2 : m_E2;
    m_prevE2 = m_E2;

    // check if we need to skip this observation
    if (m_T.isEmpty() || m_DD.isEmpty() || m_Ff.isEmpty()
            || m_N.isEmpty() || m_Nh.isEmpty() || m_VV.isEmpty() || m_E1.isEmpty()
            || m_E2.isEmpty())
    {
        return MISSING_DATA;
    }

    if (!ParseTime() || !ParseWindDir() || !ParseCloudCoverage())
    {
        return INVALID;
    }
    ParseTemper();
    ParseWindSpeed();
    ParseFog();
    ParseSnow();

    return OK;
}

bool Rp5CsvParser::ParseTime()
{
    QRegExp rx("(\\ |\\.|\\:)");
    QStringList list = m_Time.split(rx);
    int day = list.at(0).toInt();
    int month = list.at(1).toInt();
    int year = list.at(2).toInt();
    int hour = list.at(3).toInt();
    m_obs->dateTime = QDateTime(QDate(year, month, day), QTime(hour, 0, 0));

    if (!m_obs->dateTime.isValid())
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": invalid datetime \"" << m_obs->dateTime.toString(DATETIME_FORMAT) << "\"");
        return false;
    }

    return true;
}

void Rp5CsvParser::ParseTemper()
{
    m_obs->temper = m_T.toFloat();
}

bool Rp5CsvParser::ParseWindDir()
{
    if      (m_DD == "Штиль, безветрие")                          { m_obs->windDir = mm::calm; }
    else if (m_DD == "Ветер, дующий с севера")                    { m_obs->windDir = mm::N;    }
    else if (m_DD == "Ветер, дующий с северо-северо-востока")     { m_obs->windDir = mm::NNE;  }
    else if (m_DD == "Ветер, дующий с северо-востока")            { m_obs->windDir = mm::NE;   }
    else if (m_DD == "Ветер, дующий с востоко-северо-востока")    { m_obs->windDir = mm::ENE;  }
    else if (m_DD == "Ветер, дующий с востока")                   { m_obs->windDir = mm::E;    }
    else if (m_DD == "Ветер, дующий с востоко-юго-востока")       { m_obs->windDir = mm::ESE;  }
    else if (m_DD == "Ветер, дующий с юго-востока")               { m_obs->windDir = mm::SE;   }
    else if (m_DD == "Ветер, дующий с юго-юго-востока")           { m_obs->windDir = mm::SSE;  }
    else if (m_DD == "Ветер, дующий с юга")                       { m_obs->windDir = mm::S;    }
    else if (m_DD == "Ветер, дующий с юго-юго-запада")            { m_obs->windDir = mm::SSW;  }
    else if (m_DD == "Ветер, дующий с юго-запада")                { m_obs->windDir = mm::SW;   }
    else if (m_DD == "Ветер, дующий с западо-юго-запада")         { m_obs->windDir = mm::WSW;  }
    else if (m_DD == "Ветер, дующий с запада")                    { m_obs->windDir = mm::W;    }
    else if (m_DD == "Ветер, дующий с западо-северо-запада")      { m_obs->windDir = mm::WNW;  }
    else if (m_DD == "Ветер, дующий с северо-запада")             { m_obs->windDir = mm::NW;   }
    else if (m_DD == "Ветер, дующий с северо-северо-запада")      { m_obs->windDir = mm::NNW;  }
    else
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": invalid  wind direction: \"" << m_DD << "\" at " << m_obs->dateTime.toString(DATETIME_FORMAT));
        return false;
    }

    return true;
}

void Rp5CsvParser::ParseWindSpeed()
{
    m_obs->windSpeed = m_Ff.toFloat();
}

bool Rp5CsvParser::ParseCloudCoverage()
{
    int cloudAmount;
    int lowerCloudAmount;
    
    if      (m_N == "Облаков нет.")                                { cloudAmount = 0;  }
    else if (m_N == "10%  или менее, но не 0")                     { cloudAmount = 1;  }
    else if (m_N == "20–30%.")                                     { cloudAmount = 3;  }
    else if (m_N == "40%.")                                        { cloudAmount = 4;  }
    else if (m_N == "50%.")                                        { cloudAmount = 5;  }
    else if (m_N == "60%.")                                        { cloudAmount = 6;  }
    else if (m_N == "70 – 80%.")                                   { cloudAmount = 8;  }
    else if (m_N == "90  или более, но не 100%" || m_N == "100%.") { cloudAmount = 10; }
    else
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": invalid cloud coverage: \"" << m_N << "\" at " << m_obs->dateTime.toString(DATETIME_FORMAT));
        return false;
    }

    if      (m_Nh == "Облаков нет.")                                 { lowerCloudAmount = 0;  }
    else if (m_Nh == "10%  или менее, но не 0")                      { lowerCloudAmount = 1;  }
    else if (m_Nh == "20–30%.")                                      { lowerCloudAmount = 3;  }
    else if (m_Nh == "40%.")                                         { lowerCloudAmount = 4;  }
    else if (m_Nh == "50%.")                                         { lowerCloudAmount = 5;  }
    else if (m_Nh == "60%.")                                         { lowerCloudAmount = 6;  }
    else if (m_Nh == "70 – 80%.")                                    { lowerCloudAmount = 8;  }
    else if (m_Nh == "90  или более, но не 100%" || m_Nh == "100%.") { lowerCloudAmount = 10; }
    else
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": invalid lower cloud coverage: \"" << m_Nh << "\" at " << m_obs->dateTime.toString(DATETIME_FORMAT));
        return false;
    }

    m_obs->cloudAmount = cloudAmount;
    m_obs->lowerCloudAmount = lowerCloudAmount;

    return true;
}

void Rp5CsvParser::ParseFog()
{
    if (m_VV == "менее 0.05") { m_obs->fog = true; }
    else                      { m_obs->fog = m_VV.toFloat() < 1 ? true : false; }
}

void Rp5CsvParser::ParseSnow()
{
    m_obs->snow = (
        m_E2 != "Неровный слой слежавшегося или мокрого снега покрывает почву полностью." &&
        m_E2 != "Неровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        m_E2 != "Ровный слой слежавшегося или мокрого снега покрывает поверхность почвы полностью." &&
        m_E2 != "Ровный слой сухого рассыпчатого снега покрывает поверхность почвы полностью." &&
        m_E2 != "Снег покрывает поверхность почвы полностью; глубокие сугробы."
        ) ? false : true;
}
