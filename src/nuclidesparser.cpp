#include "nuclidesparser.h"

NuclidesParser::NuclidesParser()
{
    Init(2, "name", ';');
}

CsvParser::t_lineStatus NuclidesParser::ParseLine(const QString &line, mt::t_nuclideInfo &nuclideInfo,
                                                  QString& nuclideName)
{
    t_lineStatus status = CsvParser::SplitLine(line);
    if (status != OK)
    {
        return status;
    }

    bool casting = true;
    nuclideName = m_list.at(0);
    nuclideInfo.halflife = m_list.at(1).toFloat(&casting);

    if (!casting)
    {
        return INVALID;
    }

    return OK;
}
