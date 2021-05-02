#ifndef CLIMATICPARSINGSTRATEGY_H
#define CLIMATICPARSINGSTRATEGY_H

#include <QString>

#include "globaltypes.h"
#include "csvparser.h"

class ClimaticParsingStrategy
{
public:
    ClimaticParsingStrategy();
    virtual ~ClimaticParsingStrategy();
    virtual CsvParser::t_lineStatus ParseLine(const QStringList& list, mt::t_observation& observation) = 0;
protected:
    mt::t_observation* m_obs;
};

#endif // CLIMATICPARSINGSTRATEGY_H
