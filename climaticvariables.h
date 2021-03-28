#ifndef CLIMATICVARIABLES_H
#define CLIMATICVARIABLES_H

#include <QString>

#include "datamanager.h"
#include "climatecsvparser.h"

class ClimaticVariables : public DataManager
{
public:
    ClimaticVariables();
    ~ClimaticVariables();
    void AddJournal(const QString& filename, ClimateCsvParser::t_format format);
private:
    ClimateCsvParser* m_parser;
};

#endif // CLIMATICVARIABLES_H
