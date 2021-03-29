#include "geospatialcsvparser.h"

#include <QStringList>

GeospatialCsvParser::GeospatialCsvParser() :
    CsvParser(5, "system:index")
{

}

GeospatialCsvParser::t_lineStatus GeospatialCsvParser::ParseLine(const QString &string, GeospatialData::t_point &point)
{
    m_point = point;

    t_lineStatus status = CsvParser::ParseLine(string);
    if (status != OK)
    {
        return status;
    }

    return OK;
}
