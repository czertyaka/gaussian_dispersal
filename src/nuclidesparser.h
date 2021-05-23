#ifndef NUCLIDESPARSER_H
#define NUCLIDESPARSER_H

#include "csvparser.h"
#include "globaltypes.h"

class NuclidesParser : public CsvParser
{
public:
    NuclidesParser();
    t_lineStatus ParseLine(const QString &line, mt::t_nuclideInfo &nuclideInfo,
                           QString& nuclideName);
};

#endif // NUCLIDESPARSER_H
