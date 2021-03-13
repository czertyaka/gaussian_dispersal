#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DataBaseManager
{
public:
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
private:
    DataBaseManager();
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H