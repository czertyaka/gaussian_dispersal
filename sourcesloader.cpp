#include "sourcesloader.h"
#include "database.h"
#include "datainterface.h"

SourcesLoader::SourcesLoader()
{

}

bool SourcesLoader::AddSources(const SourcesLoader::t_vSources &vSources)
{
    DataBase::t_sources* sources = &(m_db.Sources());
    if(!CheckPointer(sources, "error opening sources databse"))
    {
        return false;
    }

    sources->clear();

    *sources = vSources;
    m_status = READY;

    MY_LOG(sources->size() << " sources were successfully added");

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
