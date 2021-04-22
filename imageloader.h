#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "basedataloader.h"

#include <QImage>
#include <optional>

class ImageLoader : public BaseDataLoader
{
public:
    typedef struct borders
    {
        double north;
        double east;
        double south;
        double west;
    } t_borders;
    typedef std::optional<t_borders> t_optBorders;
    typedef struct image
    {
        QImage picture;
        t_optBorders borders;
    } t_image;
    ImageLoader();
    bool AddImage(const QString& filename, const t_optBorders& optBorders);
    void Reset() override;
};

#endif // IMAGEDATA_H
