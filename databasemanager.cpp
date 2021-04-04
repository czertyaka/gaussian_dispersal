#include "databasemanager.h"

DataBaseManager &DataBaseManager::GetInstance()
{
    static DataBaseManager Instance;
    return Instance;
}

DataBaseManager::~DataBaseManager()
{
    delete m_climateJournal;
    delete m_landscape;
    delete m_image;
    delete m_sources;
}

DataBaseManager::t_climateJournal* DataBaseManager::GetClimateJournal()
{
    return m_climateJournal;
}

DataBaseManager::t_landscape* DataBaseManager::GetLandscape()
{
    return m_landscape;
}

DataBaseManager::t_image* DataBaseManager::GetImage()
{
    return m_image;
}

DataBaseManager::t_sources* DataBaseManager::GetSources()
{
    return m_sources;
}

DataBaseManager::DataBaseManager() :
    m_climateJournal(new t_climateJournal),
    m_landscape(new t_landscape),
    m_image(new t_image),
    m_sources(new t_sources)
{
}
