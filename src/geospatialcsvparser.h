#ifndef GEOSPATIALCSVPARSER_H
#define GEOSPATIALCSVPARSER_H

#include <QString>

#include "geospatialdataloader.h"
#include "csvparser.h"

class GeospatialCsvParser : public CsvParser
{
public:
    GeospatialCsvParser();
    t_lineStatus ParseLine(const QString& string, GeospatialDataLoader::t_point& point);
private:
    GeospatialDataLoader::t_microrelief ParseMicrorelief(const QString& sValue, bool* ok = nullptr);
    GeospatialDataLoader::t_point* m_point;
};

#endif // GEOSPATIALCSVPARSER_H
