#ifndef GEOSPATIALCSVPARSER_H
#define GEOSPATIALCSVPARSER_H

#include <QString>

#include "geospatialdata.h"
#include "csvparser.h"

class GeospatialCsvParser : public CsvParser
{
public:
    GeospatialCsvParser();
    t_lineStatus ParseLine(const QString& string, GeospatialData::t_point& point);
private:
    GeospatialData::t_microrelief ParseMicrorelief(const QString& sValue, bool* ok = nullptr);
    GeospatialData::t_point* m_point;
};

#endif // GEOSPATIALCSVPARSER_H
