#ifndef CLIMATECSVPARSER_H
#define CLIMATECSVPARSER_H

#include "meteorology.h"
#include "rp5csvparser.h"

class ClimateCsvParser
{
public:
    enum t_format
    {
        NONE,
        RP5
    };
    ClimateCsvParser();
    ~ClimateCsvParser();
    void SetFormat(t_format format) { m_format = format; }
    bool Parse(const QString& string, mm::t_observation& observation);
private:
    t_format m_format;
    Rp5CsvParser* m_rp5Parser;
};

#endif // CLIMATECSVPARSER_H
