#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>

#include "meteorology.h"

class DataBaseManager
{
public:
    typedef std::vector<mm::t_observation> t_climateJournal;
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    t_climateJournal& GetClimateJournal();
private:
    DataBaseManager();
    t_climateJournal m_climateJournal;
};

#endif // DATABASEMANAGER_H
