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

DataBaseManager::t_landscape &DataBaseManager::GetLandscape()
{
    return m_landscape;
}

DataBaseManager::t_sources &DataBaseManager::GetSources()
{
    return m_sources;
}

DataBaseManager::DataBaseManager()
{
}
