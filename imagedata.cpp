#include "imagedata.h"
#include "databasemanager.h"
#include "datainterface.h"

#include <QPicture>

ImageData::ImageData()
{

}

bool ImageData::AddImage(const QString &filename, const ImageData::t_optBorders& optBorders)
{
    t_image& image = m_dbManager.GetImage();

//    if (!image.picture.load(filename))
//    {
//        MY_LOG(": error loading picture " << filename);
//        m_status = NOT_READY;
//        return false;
//    }

    image.borders = optBorders;

    MY_LOG(": image " << filename << " were loaded successfully");
    m_status = READY;
    return true;
}

void ImageData::Reset()
{
    m_status = NOT_READY;
}
