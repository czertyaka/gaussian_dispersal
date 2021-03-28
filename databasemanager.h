#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <vector>

#include "climatecsvparser.h"
#include "meteorology.h"

class DataBaseManager
{
public:
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    bool AddClimateJournal(const QString& filename, ClimateCsvParser::t_format format);
private:
    DataBaseManager();
    std::vector<mm::t_observation> m_climateJournal;
    ClimateCsvParser* m_climateCsvParser;
};

#endif // DATABASEMANAGER_H
