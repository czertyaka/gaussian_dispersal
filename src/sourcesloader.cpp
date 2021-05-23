#include "sourcesloader.h"
#include "database.h"
#include "datainterface.h"

SourcesLoader::SourcesLoader()
{

}

bool SourcesLoader::AddSources(const mt::t_sourcesTable& sources, const mt::t_emissionsTable& emissions)
{
    Reset();

    db::t_sourcesTable& dbSources = m_db.Sources();
    dbSources = sources;

    db::t_emissionsTable& dbEmissions = m_db.Emissions();
    dbEmissions = emissions;

    m_status = READY;

    MY_LOG(dbSources.size() << " sources were successfully added");
    MY_LOG(dbEmissions.size() << " emissions were succesfully added");

    return true;
}

void SourcesLoader::Reset()
{
    m_db.Sources().clear();
    m_db.Emissions().clear();
    m_status = NOT_READY;
}
