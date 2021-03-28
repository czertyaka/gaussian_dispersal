#include "databasemanager.h"

DataBaseManager &DataBaseManager::GetInstance()
{
    static DataBaseManager Instance;
    return Instance;
}

DataBaseManager::~DataBaseManager()
{
}

DataBaseManager::t_climateJournal &DataBaseManager::GetClimateJournal()
{
    return m_climateJournal;
}

DataBaseManager::DataBaseManager()
{
}
