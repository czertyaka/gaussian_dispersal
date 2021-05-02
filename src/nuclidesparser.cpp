#include "nuclidesparser.h"

NuclidesParser::NuclidesParser()
{
    Init(2, "name", ';');
}

CsvParser::t_lineStatus NuclidesParser::ParseLine(const QString &string, mt::t_nuclide &nuclide)
{
    t_lineStatus status = CsvParser::SplitLine(string);
    if (status != OK)
    {
        return status;
    }

    bool casting = true;
    nuclide.name = m_list.at(0);
    nuclide.halfLife = m_list.at(1).toFloat(&casting);

    if (!casting)
    {
        return INVALID;
    }

    return OK;
}
