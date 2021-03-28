#include "datamanager.h"
#include "databasemanager.h"

DataManager::DataManager()
    : m_status(NOT_READY)
    , m_dbManager(DataBaseManager::GetInstance())
{

}
