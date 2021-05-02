#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "basedataloader.h"
#include "globaltypes.h"
#include "database.h"

class ImageLoader : public BaseDataLoader
{
public:
    ImageLoader();
    virtual ~ImageLoader() = default;
    bool AddImage(const QString& filename, const db::t_optBorders& optBorders);
    void Reset() override;
};

#endif // IMAGEDATA_H
