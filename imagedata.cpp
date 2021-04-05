#include "imagedata.h"
#include "databasemanager.h"
#include "datainterface.h"

#include <QPicture>

ImageData::ImageData()
{

}

bool ImageData::AddImage(const QString &filename, const ImageData::t_optBorders& optBorders)
{
    if (filename.isEmpty())
    {
        MY_LOG(": image filename field is empty");
        m_status = NOT_READY;
        return false;
    }

    t_image* image = m_dbManager.GetImage();
    if (!CheckPointer(image, ": error opening database for image"))
    {
        m_status = ERROR;
        return false;
    }

    if (!image->picture.load(filename))
    {
        MY_LOG(": error loading picture " << filename);
        m_status = NOT_READY;
        return false;
    }

    image->borders = optBorders;

    MY_LOG(": image " << filename << " were loaded successfully");
    m_status = READY;
    return true;
}

void ImageData::Reset()
{
    MY_LOG(": image were reseted");
    m_status = NOT_READY;
}
