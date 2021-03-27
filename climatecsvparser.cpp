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

ClimateCsvParser::t_lineStatus ClimateCsvParser::Parse(const QString& string, mm::t_observation &observation)
{
    t_lineStatus result = INVALID;

    switch (m_format)
    {
    case NONE:
        qDebug() << __PRETTY_FUNCTION__ << ": format of scv file not set";
        break;
    case RP5:
        result = m_rp5Parser->Parse(string, observation);
        break;
    }

    return result;
}
