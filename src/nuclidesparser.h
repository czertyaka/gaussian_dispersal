#ifndef NUCLIDESPARSER_H
#define NUCLIDESPARSER_H

#include "csvparser.h"
#include "globaltypes.h"

class NuclidesParser : public CsvParser
{
public:
    NuclidesParser();
    t_lineStatus ParseLine(const QString& string, mt::t_nuclide& nuclide);
};

#endif // NUCLIDESPARSER_H
