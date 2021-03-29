#ifndef CLIMATECSVPARSER_H
#define CLIMATECSVPARSER_H

#include "meteorology.h"
#include "csvparser.h"

class Rp5CsvParser;

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
    CsvParser::t_lineStatus Parse(const QString& string, mm::t_observation& observation);
private:
    t_format m_format; ///< формат обрабатываемого файла
    Rp5CsvParser* m_rp5Parser; ///< указатель на парсер CSV файла с сайта RP5
};

#endif // CLIMATECSVPARSER_H
