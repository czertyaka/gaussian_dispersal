#include "climaticvariables.h"

ClimaticVariables::ClimaticVariables()
{

}

void ClimaticVariables::AddJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    if (m_dbManager.AddClimateJournal(filename, format))
    {
        m_status = READY;
    }
    else
    {
        m_status = ERROR;
    }
}
