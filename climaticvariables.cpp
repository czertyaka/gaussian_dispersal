#include "climaticvariables.h"

ClimaticVariables::ClimaticVariables()
{

}

void ClimaticVariables::AddJournal(const QString &filename)
{
    if (m_dbManager.AddClimateJournal(filename))
    {
        m_status = READY;
    }
    else
    {
        m_status = ERROR;
    }
}
