#ifndef CLIMATICVARIABLES_H
#define CLIMATICVARIABLES_H

#include <QString>

#include "datamanager.h"

class ClimaticVariables : public DataManager
{
public:
    ClimaticVariables();
    void AddJournal(const QString& filename);
};

#endif // CLIMATICVARIABLES_H
