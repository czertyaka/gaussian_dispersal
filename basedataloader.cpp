#include "basedataloader.h"
#include "database.h"
#include "datainterface.h"

BaseDataLoader::BaseDataLoader()
    : m_status(NOT_READY)
    , m_db(DataBase::GetInstance())
{

}

bool BaseDataLoader::CheckPointer(void *ptr, const QString &msg)
{
    if (!ptr)
    {
        MY_LOG(msg);
        m_status = ERROR;
        return false;
    }

    return true;
}
