#include "sourcesdata.h"
#include "databasemanager.h"
#include "datainterface.h"

SourcesData::SourcesData()
{

}

bool SourcesData::AddSources(const SourcesData::t_vSources &vSources)
{
    DataBaseManager::t_sources& sources = m_dbManager.GetSources();
    sources.clear();

    sources = vSources;
    m_status = READY;

    MY_LOG(": " << sources.size() << " sources were successfully added");

    return true;
}

void SourcesData::Reset()
{
    DataBaseManager::t_sources& sources = m_dbManager.GetSources();
    sources.clear();
    m_status = NOT_READY;
}
