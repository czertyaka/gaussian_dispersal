#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>

#include "databasemanager.h"
#include "meteorology.h"

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

bool DataBaseManager::AddClimateJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    m_climateCsvParser->SetFormat(format);

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << __PRETTY_FUNCTION__ << ": error opening file";
    }
    else
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            mm::t_observation observation;
            if (m_climateCsvParser->Parse(line, observation))
            {
                //TODO add to database
            }
        }
    }

    return true;
}

DataBaseManager::DataBaseManager()
    : m_db(QSqlDatabase::addDatabase("QSQLITE"))
    , m_climateCsvParser(new ClimateCsvParser)
{
    m_db.setDatabaseName("scattering_db.sqlite");
    if (m_db.open())
    {
        qDebug() << __PRETTY_FUNCTION__ << ": database opened successfully";
        Init();
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": database not open";
    }
}

void DataBaseManager::Init()
{
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS climate_journal;");

    if (query.exec())
    {
        qDebug() << __PRETTY_FUNCTION__ << ": climate jornal table created successfully";
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ": climate journal table creation error:" << query.lastError().text();
    }
}
