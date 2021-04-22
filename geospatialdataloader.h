#ifndef GeospatialDataLoader_H
#define GeospatialDataLoader_H

#include <QString>

#include "basedataloader.h"

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
        float x;
        float y;
        t_microrelief microrelief;
        short int elevation;
    } t_point;
    GeospatialDataLoader();
    ~GeospatialDataLoader();
    bool AddFromFile(const QString& filename);
    void Reset() override;
private:
    GeospatialCsvParser* m_parser;
};

#endif // GeospatialDataLoader_H
