#include "sourcesdata.h"
#include "databasemanager.h"

SourcesData::SourcesData()
{

}

bool SourcesData::AddSources(const SourcesData::t_vSources &vSources)
{
    DataBaseManager::t_sources& sources = m_dbManager.GetSources();
    sources.clear();

    sources = vSources;
    m_status = READY;
    return true;
}
