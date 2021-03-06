#include "imageloader.h"
#include "database.h"
#include "datainterface.h"

#include <QPicture>

ImageLoader::ImageLoader()
{

}

bool ImageLoader::AddImage(const QString &filename, const mt::t_optBorders& optBorders)
{
    if (filename.isEmpty())
    {
        MY_LOG("image filename field is empty");
        m_status = NOT_READY;
        return false;
    }

    mt::t_image& image = m_db.Image();

    if (!image.picture.load(filename))
    {
        MY_LOG("error loading picture " << filename);
        m_status = NOT_READY;
        return false;
    }

    image.borders = optBorders;

    MY_LOG("image " << filename << " were loaded successfully");
    m_status = READY;
    return true;
}

void ImageLoader::Reset()
{
    MY_LOG("image were reseted");
    m_status = NOT_READY;
}
