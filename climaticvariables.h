#ifndef CLIMATICVARIABLES_H
#define CLIMATICVARIABLES_H

#include <QString>

#include "datamanager.h"
#include "climatecsvparser.h"

class ClimaticVariables : public DataManager
{
public:
    ClimaticVariables();
    void AddJournal(const QString& filename, ClimateCsvParser::t_format format);
};

#endif // CLIMATICVARIABLES_H
