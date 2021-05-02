#include "sourcesloader.h"
#include "database.h"
#include "datainterface.h"

SourcesLoader::SourcesLoader()
{

}

bool SourcesLoader::AddSources(const DataBase::t_sources &sources)
{
    DataBase::t_sources* pSources = &(m_db.Sources());
    if(!CheckPointer(pSources, "error opening sources database"))
    {
        return false;
    }

    pSources->clear();

    *pSources = sources;
    m_status = READY;

    MY_LOG(pSources->size() << " sources were successfully added");

    return true;
}

void SourcesLoader::Reset()
{
    DataBase::t_sources* sources = &(m_db.Sources());
    if (CheckPointer(sources, "error opening sources databse"))
    {
        sources->clear();
        m_status = NOT_READY;
    }
}
