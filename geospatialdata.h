#ifndef GEOSPATIALDATA_H
#define GEOSPATIALDATA_H

#include <QString>

#include "datamanager.h"

class GeospatialCsvParser;

class GeospatialData : public DataManager
{
public:
    enum t_microrelief
    {
        SNOW,
        SHORTGRASS,
        TALLGRASS,
        SCRUB_GROWTH,
        FOREST,
        BUILDINGS
    };
    typedef struct point
    {
        float longitude;
        float latitude;
        t_microrelief microrelief;
        short int elevation;
    } t_point;
    GeospatialData();
    ~GeospatialData();
    void AddFromFile(const QString& filename);
private:
    GeospatialCsvParser* m_parser;
};

#endif // GEOSPATIALDATA_H
