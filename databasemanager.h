#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DataBaseManager
{
public:
    static DataBaseManager& GetInstance();
    ~DataBaseManager();
    bool AddClimateJournal(const QString& filename);
private:
    DataBaseManager();
    void Init();
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
