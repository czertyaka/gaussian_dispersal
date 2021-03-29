#ifndef GEOSPATIALDATA_H
#define GEOSPATIALDATA_H

#include <QString>

#include "datamanager.h"

class GeospatialCsvParser;

class GeospatialData : public DataManager
{
public:
    enum t_microrelief : unsigned short
    {
        UNKNOWN,
        SNOW,
        SHORTGRASS,
        TALLGRASS,
        SCRUB_GROWTH,
        FOREST,
        BUILDINGS,
        WATER
    };
    typedef struct point
    {
        float x;
        float y;
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
