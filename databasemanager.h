#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>

#include "meteorology.h"
#include "geospatialdata.h"
#include "sourcesdata.h"

class DataBaseManager
{
public:
    typedef std::vector<mm::t_observation> t_climateJournal;
    typedef std::vector<GeospatialData::t_point> t_landscape;
    typedef SourcesData::t_vSources t_sources;
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    t_climateJournal& GetClimateJournal();
    t_landscape& GetLandscape();
    t_sources& GetSources();
private:
    DataBaseManager();
    t_climateJournal m_climateJournal;
    t_landscape m_landscape;
    t_sources m_sources;
};

#endif // DATABASEMANAGER_H
