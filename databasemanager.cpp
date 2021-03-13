#include "databasemanager.h"

DataBaseManager &DataBaseManager::GetInstance()
{
    static DataBaseManager Instance;
    return Instance;
}

DataBaseManager::~DataBaseManager()
{
    m_db.close();
}

bool DataBaseManager::AddClimateJournal(const QString &filename)
{
    //TODO
    return true;
}

DataBaseManager::DataBaseManager()
    : m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    m_db.setDatabaseName("scattering_db.sqlite");
    m_db.open();
}
