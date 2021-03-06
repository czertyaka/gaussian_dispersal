#include "geospatialcsvparser.h"
#include "datainterface.h"

#include <QStringList>

GeospatialCsvParser::GeospatialCsvParser() :
    CsvParser()
{
    Init(5, "system:index", ',');
}

GeospatialCsvParser::t_lineStatus GeospatialCsvParser::ParseLine(const QString &string, mt::t_point &point)
{   
    m_point = &point;

    t_lineStatus status = CsvParser::SplitLine(string);
    if (status != OK)
    {
        return status;
    }

    bool casting = true;
    m_point->coord.lat = m_list.at(3).toFloat(&casting);
    m_point->coord.lon = m_list.at(4).toFloat(&casting);
    m_point->elevation = m_list.at(2).toShort(&casting);
    m_point->microrelief = ParseMicrorelief(m_list.at(1), &casting);

    if (!casting || m_point->microrelief == mt::MR_UNKNOWN)
    {
        return INVALID;
    }

    return OK;
}

mt::t_microrelief GeospatialCsvParser::ParseMicrorelief(const QString &sValue, bool *ok)
{
    enum localTypes : unsigned short
    {
        UNKNOWN = 0,
        SHRUB = 20,
        GRASS = 30,
        AGRICULTURE = 40,
        BUILDINGS = 50,
        THIN_VEGETATION = 60,
        SNOW = 70,
        WATER = 80,
        SWAMPS = 90,
        MOSS = 100,
        FOREST_CLOSED_EVERGREEN_ACICULAR = 111,
        FOREST_CLOSED_EVERGREEN_BROADLEAVED = 112,
        FOREST_CLOSED_DECIDIOUS_ACICULAR = 113,
        FOREST_CLOSED_DECIDIOUS_BROADLEAVED = 114,
        FOREST_CLOSED_MIXED = 115,
        FOREST_CLOSED_UNKNOWN = 116,
        FOREST_OPENED_EVERGREEN_ACICULAR = 121,
        FOREST_OPENED_EVERGREEN_BROADLEAVED = 122,
        FOREST_OPENED_DECIDIOUS_ACICULAR = 123,
        FOREST_OPENED_DECIDIOUS_BROADLEAVED = 124,
        FOREST_OPENED_MIXED = 125,
        FOREST_OPENED_UNKNOWN = 126,
        SEAS = 200
    };

    short value = sValue.toUShort(ok);
    if (!(*ok))
    {
        return mt::MR_UNKNOWN;
    }

    switch (value)
    {
    case SNOW:
        return mt::MR_SNOW;
    case GRASS:
    case THIN_VEGETATION:
    case MOSS:
        return mt::MR_SHORTGRASS;
    case AGRICULTURE:
    case SWAMPS:
        return mt::MR_TALLGRASS;
    case SHRUB:
        return mt::MR_SCRUB_GROWTH;
    case BUILDINGS:
        return mt::MR_BUILDINGS;
    case WATER:
    case SEAS:
        return mt::MR_WATER;
    case FOREST_CLOSED_EVERGREEN_ACICULAR:
    case FOREST_CLOSED_EVERGREEN_BROADLEAVED:
    case FOREST_CLOSED_DECIDIOUS_ACICULAR:
    case FOREST_CLOSED_DECIDIOUS_BROADLEAVED:
    case FOREST_CLOSED_MIXED:
    case FOREST_CLOSED_UNKNOWN:
    case FOREST_OPENED_EVERGREEN_ACICULAR:
    case FOREST_OPENED_EVERGREEN_BROADLEAVED:
    case FOREST_OPENED_DECIDIOUS_ACICULAR:
    case FOREST_OPENED_DECIDIOUS_BROADLEAVED:
    case FOREST_OPENED_MIXED:
    case FOREST_OPENED_UNKNOWN:
        return mt::MR_FOREST;
    case UNKNOWN:
        return mt::MR_UNKNOWN;
    default:
        *ok = false;
        MY_LOG("invalid microrelief type: \"" << sValue
               << "\"");
        return mt::MR_UNKNOWN;
    }
}
