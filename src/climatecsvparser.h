#ifndef CLIMATECSVPARSER_H
#define CLIMATECSVPARSER_H

#include "globaltypes.h"
#include "csvparser.h"

class ClimaticParsingStrategy;

class ClimateCsvParser : public CsvParser
{
public:
    enum t_format
    {
        NONE,
        RP5
    };

    ClimateCsvParser();
    ~ClimateCsvParser();
    void SetFormat(t_format format);
    t_lineStatus ParseLine(const QString& string, mt::t_observation& observation);
private:
    t_format m_format; ///< формат обрабатываемого файла
    ClimaticParsingStrategy* m_strategy; ///< parsing strategy base class
};

#endif // CLIMATECSVPARSER_H
