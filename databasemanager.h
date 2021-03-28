#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "climatecsvparser.h"

class DataBaseManager
{
public:
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    bool AddClimateJournal(const QString& filename, ClimateCsvParser::t_format format);
private:
    DataBaseManager();
    QSqlDatabase m_db;
    QSqlQuery m_query;
    ClimateCsvParser* m_climateCsvParser;
};

#endif // DATABASEMANAGER_H
