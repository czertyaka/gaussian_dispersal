#ifndef GeospatialDataLoader_H
#define GeospatialDataLoader_H

#include <QString>

#include "basedataloader.h"
#include "meteorology.h"

class GeospatialCsvParser;

class GeospatialDataLoader : public BaseDataLoader
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
        mm::t_epsg4326coord coord;
        t_microrelief microrelief;
        short int elevation;
    } t_point;
    GeospatialDataLoader();
    virtual ~GeospatialDataLoader();
    bool AddFromFile(const QString& filename);
    void Reset() override;
private:
    GeospatialCsvParser* m_parser;
};

#endif // GeospatialDataLoader_H
