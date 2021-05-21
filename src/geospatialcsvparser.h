#ifndef GEOSPATIALCSVPARSER_H
#define GEOSPATIALCSVPARSER_H

#include <QString>

#include "geospatialdataloader.h"
#include "csvparser.h"
#include "databasetypes.h"

class GeospatialCsvParser : public CsvParser
{
public:
    GeospatialCsvParser();
    t_lineStatus ParseLine(const QString& string, dbt::t_point& point);
private:
    dbt::t_microrelief ParseMicrorelief(const QString& sValue, bool* ok = nullptr);
    dbt::t_point* m_point;
};

#endif // GEOSPATIALCSVPARSER_H
