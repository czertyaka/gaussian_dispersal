#ifndef RP5CSVPARSER_H
#define RP5CSVPARSER_H

#include "meteorology.h"

class Rp5CsvParser
{
public:
    Rp5CsvParser();
    bool Parse(const QString& string, mm::t_observation& observation);
};

#endif // RP5CSVPARSER_H
