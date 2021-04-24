#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "basedataloader.h"
#include "meteorology.h"

#include <QImage>
#include <optional>

class ImageLoader : public BaseDataLoader
{
public:
    typedef struct borders
    {
        mm::t_pseudoMercatorCoord nw; // nord west
        mm::t_pseudoMercatorCoord ne; // nord east
        mm::t_pseudoMercatorCoord sw; // south west
        mm::t_pseudoMercatorCoord se; // south east
    } t_borders;
    typedef std::optional<t_borders> t_optBorders;
    typedef struct image
    {
        QImage picture;
        t_optBorders borders;
    } t_image;
    ImageLoader();
    virtual ~ImageLoader() = default;
    bool AddImage(const QString& filename, const t_optBorders& optBorders);
    void Reset() override;
};

#endif // IMAGEDATA_H
