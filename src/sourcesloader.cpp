#include "sourcesloader.h"
#include "database.h"
#include "datainterface.h"

SourcesLoader::SourcesLoader()
{

}

bool SourcesLoader::AddSources(const DataBase::t_sourcesTable &sources)
{
    DataBase::t_sourcesTable& dbSources = m_db.Sources();

    dbSources.clear();

    dbSources = sources;
    m_status = READY;

    MY_LOG(dbSources.size() << " sources were successfully added");

    return true;
}

void SourcesLoader::Reset()
{
    m_db.Sources().clear();
    m_status = NOT_READY;
}
