#ifndef GEOSPATIALCSVPARSER_H
#define GEOSPATIALCSVPARSER_H

#include <QString>

#include "geospatialdataloader.h"
#include "csvparser.h"

class GeospatialCsvParser : public CsvParser
{
public:
    GeospatialCsvParser();
    t_lineStatus ParseLine(const QString& string, mt::t_point& point);
private:
    mt::t_microrelief ParseMicrorelief(const QString& sValue, bool* ok = nullptr);
    mt::t_point* m_point;
};

#endif // GEOSPATIALCSVPARSER_H
