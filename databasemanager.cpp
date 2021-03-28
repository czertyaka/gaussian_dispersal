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
}

bool DataBaseManager::AddClimateJournal(const QString &filename, ClimateCsvParser::t_format format)
{
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
                m_climateJournal.push_back(observation);
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

        if (!addedObservationsCounter)
        {
            MY_LOG(__PRETTY_FUNCTION__ << ": no observations were added");
            return false;
        }
    }

    file.close();
    return true;
}

DataBaseManager::DataBaseManager()
    : m_climateCsvParser(new ClimateCsvParser)
{
}
