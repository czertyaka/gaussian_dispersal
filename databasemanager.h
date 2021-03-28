#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>

#include "meteorology.h"
#include "geospatialdata.h"

class DataBaseManager
{
public:
    typedef std::vector<mm::t_observation> t_climateJournal;
    typedef std::vector<GeospatialData::t_point> t_landscape;
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    t_climateJournal& GetClimateJournal();
    t_landscape& GetLandscape();
private:
    DataBaseManager();
    t_climateJournal m_climateJournal;
    t_landscape m_landscape;
};

#endif // DATABASEMANAGER_H
