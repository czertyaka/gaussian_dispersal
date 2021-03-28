#ifndef GEOSPATIALCSVPARSER_H
#define GEOSPATIALCSVPARSER_H

#include <QString>

#include "geospatialdata.h"

class GeospatialCsvParser
{
public:
    enum t_lineStatus
    {
        OK,
        ERROR,
        NOT_A_DATA
    };
    GeospatialCsvParser();
    t_lineStatus ParseLine(const QString& string, GeospatialData::t_point& point);
};

#endif // GEOSPATIALCSVPARSER_H
