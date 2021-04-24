#include "csvparser.h"

CsvParser::CsvParser()
{

}

void CsvParser::Init(const size_t columnsNumber, const QString &firstHeader, const char delimeter)
{
    m_columnsNumber = columnsNumber;
    m_firstHeader = firstHeader;
    m_delimeter = delimeter;
}

CsvParser::t_lineStatus CsvParser::SplitLine(const QString &string)
{
    // skip comments
    if (string.at(0) == '#')
    {
        return NOT_A_DATA;
    }

    // remove delimeters and split
    QString myString(string);
    myString.remove('"');

    m_list = myString.split(m_delimeter);
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
