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
        m_query.exec("DROP TABLE IF EXISTS climate_journal;");
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

    m_query.exec("DROP TABLE IF EXISTS climate_journal;");

    m_query.prepare("CREATE TABLE climate_journal ("
               "dateTime DATETIME PRIMARY KEY,"
               "windDir INT,"
               "windSpeed REAL,"
               "cloudAmount INT,"
               "lowerCloudAmount INT,"
               "fog BOOL,"
               "snow BOOL,"
               "temper REAL"
               ");"
    );

    if (m_query.exec())
    {
        MY_LOG( __PRETTY_FUNCTION__ << ": climate jornal table created successfully");
    }
    else
    {
        MY_LOG( __PRETTY_FUNCTION__ << ": climate journal table creation error:" << m_query.lastError().text());
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

        m_query.prepare("INSERT INTO climate_journal("
                        "dateTime,"
                        "windDir,"
                        "windSpeed,"
                        "cloudAmount,"
                        "lowerCloudAmount,"
                        "fog,"
                        "snow,"
                        "temper"
                        ") VALUES("
                        ":dateTime,"
                        ":windDir,"
                        ":windSpeed,"
                        ":cloudAmount,"
                        ":lowerCloudAmount,"
                        ":fog,"
                        ":snow,"
                        ":temper"
                        ");");

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
                m_query.bindValue(":dateTime", observation.dateTime.toString(DATETIME_FORMAT));
                m_query.bindValue(":windDir", observation.windDir);
                m_query.bindValue(":windSpeed", observation.windSpeed);
                m_query.bindValue(":cloudAmount", observation.cloudAmount);
                m_query.bindValue(":lowerCloudAmount", observation.lowerCloudAmount);
                m_query.bindValue(":fog", observation.fog);
                m_query.bindValue(":snow", observation.snow);
                m_query.bindValue(":temper", observation.temper);

                if (!m_query.exec())
                {
                    MY_LOG( __PRETTY_FUNCTION__ << ": error inserting observation at "
                        << observation.dateTime.toString(DATETIME_FORMAT)
                        << ": " << m_query.lastError().text());
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
    m_db.exec("PRAGMA synchronous = OFF");
    m_db.exec("PRAGMA journal_mode = MEMORY");
}
