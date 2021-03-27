#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>

#include "databasemanager.h"
#include "meteorology.h"
#include "datainterface.h"

DataBaseManager &DataBaseManager::GetInstance()
{
    static DataBaseManager Instance;
    return Instance;
}

DataBaseManager::~DataBaseManager()
{
    if(m_db.open())
    {
        QSqlQuery dropClimateJournalQuery("DROP TABLE IF EXISTS climate_journal;");
        m_db.close();
    }
}

bool DataBaseManager::AddClimateJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    if (!m_db.open())
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": database not found");
        return false;
    }

    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS climate_journal;");

    query.prepare("CREATE TABLE climate_journal ("
               "dateTime DATETIME PRIMARY KEY NOT NULL,"
               "windDir INT NOT NULL,"
               "windSpeed REAL NOT NULL,"
               "cloudAmount INT NOT NULL,"
               "lowerCloudAmount INT NOT NULL,"
               "fog BOOL NOT NULL,"
               "snow BOOL  NOT NULL,"
               "temper REAL NOT NULL"
               ");"
    );

    if (query.exec())
    {
        MY_LOG( __PRETTY_FUNCTION__ << ": climate jornal table created successfully");
    }
    else
    {
        MY_LOG( __PRETTY_FUNCTION__ << ": climate journal table creation error:" << query.lastError().text());
        return false;
    }

    m_climateCsvParser->SetFormat(format);

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        MY_LOG( __PRETTY_FUNCTION__ << ": error opening file");
    }
    else
    {
        size_t totalObservationsCounter = 0;
        size_t addedObservationsCounter = 0;
        size_t invalidObseravtionsCounter = 0;
        size_t incompleteObservationsCounter = 0;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            mm::t_observation observation;
            ClimateCsvParser::t_lineStatus lineStatus = m_climateCsvParser->Parse(line, observation);

            if (lineStatus != ClimateCsvParser::NOT_A_DATA)
            {
                totalObservationsCounter++;
            }

            if (lineStatus == ClimateCsvParser::OK)
            {
                QSqlQuery query;
                QString str = QString("INSERT INTO climate_journal("
                    "dateTime, windDir, windSpeed, cloudAmount,"
                    "lowerCloudAmount, fog, snow, temper"
                    ") VALUES('%1', %2, %3, %4, %5, %6, %7, %8);")
                    .arg(observation.dateTime.toString(DATETIME_FORMAT))
                    .arg(observation.windDir)
                    .arg(observation.windSpeed)
                    .arg(observation.cloudAmount)
                    .arg(observation.lowerCloudAmount)
                    .arg(observation.fog)
                    .arg(observation.snow)
                    .arg(observation.temper);
                query.prepare(str);

                if (!query.exec())
                {
                    MY_LOG( __PRETTY_FUNCTION__ << ": error inserting observation at "
                        << observation.dateTime.toString(DATETIME_FORMAT)
                        << ": " << query.lastError().text());
                    return false;
                }

                addedObservationsCounter++;
            }
            else if (lineStatus == ClimateCsvParser::INVALID)
            {
                invalidObseravtionsCounter++;
            }
            else if (lineStatus == ClimateCsvParser::MISSING_DATA)
            {
                incompleteObservationsCounter++;
            }
        }

        MY_LOG( __PRETTY_FUNCTION__ << ": total obseravtions: " << totalObservationsCounter
            << "; added: " << addedObservationsCounter
            << "; invalid: " << invalidObseravtionsCounter
            << "; incomplete: " << incompleteObservationsCounter);
    }

    file.close();
    return true;
}

DataBaseManager::DataBaseManager()
    : m_db(QSqlDatabase::addDatabase("QSQLITE"))
    , m_climateCsvParser(new ClimateCsvParser)
{
    m_db.setDatabaseName("scattering_db.sqlite");
}
