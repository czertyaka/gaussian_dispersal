#include "QDebug"

#include "climatecsvparser.h"
#include "rp5csvparser.h"

ClimateCsvParser::ClimateCsvParser()
    : m_format(NONE)
    , m_rp5Parser(new Rp5CsvParser)
{

}

ClimateCsvParser::~ClimateCsvParser()
{
    delete m_rp5Parser;
}

CsvParser::t_lineStatus ClimateCsvParser::Parse(const QString& string, mm::t_observation &observation)
{
    CsvParser::t_lineStatus result = CsvParser::INVALID;

    switch (m_format)
    {
    case NONE:
        qDebug() << ": format of csv file not set";
        break;
    case RP5:
        result = m_rp5Parser->Parse(string, observation);
        break;
    }

    return result;
}
