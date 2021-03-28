#include "climaticvariables.h"
#include "datainterface.h"
#include "databasemanager.h"

#include <QFile>

ClimaticVariables::ClimaticVariables()
    : m_parser(new ClimateCsvParser())
{
    if (!m_parser)
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": error creating climate journal parser");
    }

    m_status = ERROR;
}

ClimaticVariables::~ClimaticVariables()
{
    delete m_parser;
}

void ClimaticVariables::AddJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    m_parser->SetFormat(format);
    DataBaseManager::t_climateJournal climateJournal = m_dbManager.GetClimateJournal();

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
            ClimateCsvParser::t_lineStatus lineStatus = m_parser->Parse(line, observation);

            if (lineStatus != ClimateCsvParser::NOT_A_DATA)
            {
                totalObservationsCounter++;
            }
            else
            {
                continue;
            }

            if (lineStatus == ClimateCsvParser::OK)
            {
                climateJournal.push_back(observation);
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
            MY_LOG(__PRETTY_FUNCTION__ << ": error reading climatic journal");
            m_status = ERROR;
            return;
        }
    }

    file.close();
    MY_LOG(__PRETTY_FUNCTION__ << ": climatic journal read successfully");
    m_status = READY;
}
