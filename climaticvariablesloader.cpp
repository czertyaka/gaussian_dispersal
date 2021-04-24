#include "climaticvariablesloader.h"
#include "datainterface.h"
#include "database.h"
#include "basedataloader.h"

#include <QFile>

ClimaticVariablesLoader::ClimaticVariablesLoader()
    : m_parser(new ClimateCsvParser())
{
    if (!m_parser)
    {
        MY_LOG("error creating climate journal parser");
    }

    m_status = ERROR;
}

ClimaticVariablesLoader::~ClimaticVariablesLoader()
{
    delete m_parser;
}

bool ClimaticVariablesLoader::AddJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    m_parser->SetFormat(format);
    DataBase::t_climateJournal* climateJournal = m_db.GetClimateJournal();

    if (filename.isEmpty())
    {
        MY_LOG("climate filename field is empty");
        return false;
    }

    if (!CheckPointer(climateJournal, "error opening climate journal database"))
    {
        return false;
    }

    climateJournal->clear();

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        MY_LOG( "error opening file");
        return false;
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
            CsvParser::t_lineStatus lineStatus = m_parser->ParseLine(line, observation);

            if (lineStatus == CsvParser::COLUMNS_MISMATCH)
            {
                MY_LOG("column mismatch in line \""
                       << line << "\", RP5 file consists of at least 29 columns");
                m_status = ERROR;
                return false;
            }

            if (lineStatus != CsvParser::NOT_A_DATA)
            {
                totalObservationsCounter++;
            }
            else
            {
                continue;
            }

            if (lineStatus == CsvParser::OK)
            {
                climateJournal->push_back(observation);
                addedObservationsCounter++;
            }
            else if (lineStatus == CsvParser::INVALID)
            {
                invalidObseravtionsCounter++;
            }
            else if (lineStatus == CsvParser::MISSING_DATA)
            {
                incompleteObservationsCounter++;
            }
        }

        MY_LOG( "total obseravtions: " << totalObservationsCounter
            << "; added: " << addedObservationsCounter
            << "; invalid: " << invalidObseravtionsCounter
            << "; incomplete: " << incompleteObservationsCounter);

        if (!addedObservationsCounter)
        {
            MY_LOG("no observations were added");
            MY_LOG("error reading climatic journal");
            m_status = ERROR;
            return false;
        }
    }

    file.close();
    MY_LOG("climatic journal read successfully");
    m_status = READY;
    return true;
}

void ClimaticVariablesLoader::Reset()
{
    DataBase::t_climateJournal* climateJournal = m_db.GetClimateJournal();
    if (CheckPointer(climateJournal, "error opening climate journal database"))
    {
        climateJournal->clear();
        m_status = NOT_READY;
    }
}
