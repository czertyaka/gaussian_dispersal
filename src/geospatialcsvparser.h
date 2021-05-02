#ifndef GEOSPATIALCSVPARSER_H
#define GEOSPATIALCSVPARSER_H

#include <QString>

#include "geospatialdataloader.h"
#include "csvparser.h"
#include "database.h"

class GeospatialCsvParser : public CsvParser
{
public:
    GeospatialCsvParser();
    t_lineStatus ParseLine(const QString& string, DataBase::t_point& point);
private:
    DataBase::t_microrelief ParseMicrorelief(const QString& sValue, bool* ok = nullptr);
    DataBase::t_point* m_point;
};

#endif // GEOSPATIALCSVPARSER_H
