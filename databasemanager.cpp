#include <QSqlQuery>

#include "databasemanager.h"

DataBaseManager &DataBaseManager::GetInstance()
{
    static DataBaseManager Instance;
    return Instance;
}

DataBaseManager::~DataBaseManager()
{
    QSqlQuery dropClimateJournalQuery("DROP TABLE IF EXISTS climate_journal;");
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
    if (m_db.open())
    {
        Init();
    }
    else
    {
        //TODO
    }
}

void DataBaseManager::Init()
{
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS climate_journal;");
    query.exec(
        "CREATE TABLE IF NOT EXISTS climate_journal ("
            "local_time TEXT PRIMARY KEY,"
            "temperature REAL DEFAULT NULL,"
            "pressure_station REAL DEFAULT NULL,"
            "pressure_sea_level REAL DEFAULT NULL,"
            "barometric_tendency REAL DEFAULT NULL,"
            "relative_humidity INT DEFAULT NULL,"
            "wind_direction TEXT DEFAULT NULL,"
            "wind_speed INT DEFAULT NULL,"
            "max_wind_speed_10 INT DEFAULT NULL,"
            "max_wind_speed_period INT DEFAULT NULL,"
            "total_cloud_cover TEXT DEFAULT NULL,"
            "cur_cloud_cover TEXT DEFAULT NULL,"
            "cur_cloud_cover_period_1 TEXT DEFAULT NULL,"
            "cur_cloud_cover_period_2 TEXT DEFAULT NULL,"
            "min_temperature REAL DEFAULT NULL,"
            "max_temperature REAL DEFAULT NULL,"
            "cloud_types_1 TEXT DEFAULT NULL,"
            "cloud_cover_by_type TEXT DEFAULT NULL,"
            "cloud_height TEXT DEFAULT NULL,"
            "cloud_types_2 TEXT DEFAULT NULL,"
            "cloud_types_3 TEXT DEFAULT NULL,"
            "visible_range REAL DEFAULT NULL,"
            "dewpoint_temperature REAL DEFAULT NULL,"
            "precipitation TEXT DEFAULT NULL,"
            "precipitation_period INT DEFAULT NULL,"
            "ground_condition_no_snow TEXT DEFAULT NULL,"
            "min_ground_temp INT DEFAULT NULL,"
            "ground_condition_snow TEXT DEFAULT NULL,"
            "snow_cover_height TEXT DEFAULT NULL"
        ");");
}
