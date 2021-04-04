#include "datamanager.h"
#include "databasemanager.h"
#include "datainterface.h"

DataManager::DataManager()
    : m_status(NOT_READY)
    , m_dbManager(DataBaseManager::GetInstance())
{

}

bool DataManager::CheckPointer(void *ptr, const QString &msg)
{
    if (!ptr)
    {
        MY_LOG(msg);
        m_status = ERROR;
        return false;
    }

    return true;
}
