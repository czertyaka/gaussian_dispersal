#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

#include "climatecsvparser.h"

class DataBaseManager
{
public:
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    bool AddClimateJournal(const QString& filename, ClimateCsvParser::t_format format);
private:
    DataBaseManager();
    void Init();
    QSqlDatabase m_db;
    ClimateCsvParser* m_climateCsvParser;
};

#endif // DATABASEMANAGER_H
