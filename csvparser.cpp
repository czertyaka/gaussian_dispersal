#include "csvparser.h"

CsvParser::CsvParser(const size_t columnsNumber, const QString& firstHeader) :
    m_columnsNumber(columnsNumber),
    m_firstHeader(firstHeader)
{

}

CsvParser::t_lineStatus CsvParser::ParseLine(const QString &string)
{
    // skip comments
    if (string.at(0) == '#')
    {
        return NOT_A_DATA;
    }

    // remove qutes and split
    QString myString(string);
    myString.remove('"');

    m_list = myString.split(";");
    if (m_list.count() < m_columnsNumber)
    {
        return COLUMNS_MISMATCH;
    }

    // skip header
    if (m_list.at(0).contains(m_firstHeader))
    {
        return NOT_A_DATA;
    }

    return OK;
}
