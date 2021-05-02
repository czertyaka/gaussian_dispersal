#ifndef GeospatialDataLoader_H
#define GeospatialDataLoader_H

#include <QString>

#include "basedataloader.h"
#include "globaltypes.h"

class GeospatialCsvParser;

class GeospatialDataLoader : public BaseDataLoader
{
public:
    GeospatialDataLoader();
    virtual ~GeospatialDataLoader();
    bool AddFromFile(const QString& filename);
    void Reset() override;
private:
    GeospatialCsvParser* m_parser;
};

#endif // GeospatialDataLoader_H
