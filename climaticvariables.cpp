#include "climaticvariables.h"
#include "datainterface.h"

ClimaticVariables::ClimaticVariables()
{

}

void ClimaticVariables::AddJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    if (m_dbManager.AddClimateJournal(filename, format))
    {
        m_status = READY;
        MY_LOG(__PRETTY_FUNCTION__ << ": climatic journal read successfully");
    }
    else
    {
        m_status = ERROR;
        MY_LOG(__PRETTY_FUNCTION__ << ": error reading climatic journal");
    }
}
