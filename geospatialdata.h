#ifndef GEOSPATIALDATA_H
#define GEOSPATIALDATA_H

#include <QString>

#include "datamanager.h"

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
    typedef struct geospatialData
    {
        float longitude;
        float latitude;
        t_microrelief microrelief;
        short int elevation;
    } t_geospatialData;
    GeospatialData();
    void AddFromFile(const QString& filename);
};

#endif // GEOSPATIALDATA_H
