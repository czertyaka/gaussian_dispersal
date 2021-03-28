#ifndef CLIMATECSVPARSER_H
#define CLIMATECSVPARSER_H

#include "meteorology.h"

class Rp5CsvParser;

class ClimateCsvParser
{
public:
    enum t_format
    {
        NONE,
        RP5
    };
    enum t_lineStatus
    {
        OK = 0,
        MISSING_DATA,
        INVALID,
        NOT_A_DATA,
        COLUMNS_MISMATCH
    };

    ClimateCsvParser();
    ~ClimateCsvParser();
    void SetFormat(t_format format) { m_format = format; }
    t_lineStatus Parse(const QString& string, mm::t_observation& observation);
private:
    t_format m_format; ///< формат обрабатываемого файла
    Rp5CsvParser* m_rp5Parser; ///< указатель на парсер CSV файла с сайта RP5
};

#endif // CLIMATECSVPARSER_H
