#include "QDebug"

#include "climatecsvparser.h"
#include "climaticparsingstrategy.h"
#include "rp5strategy.h"

ClimateCsvParser::ClimateCsvParser()
    : CsvParser()
    , m_format(NONE)
    , m_strategy(nullptr)
{

}

ClimateCsvParser::~ClimateCsvParser()
{
    if (m_strategy)
    {
        delete m_strategy;
    }
}

void ClimateCsvParser::SetFormat(ClimateCsvParser::t_format format)
{
    if (m_strategy)
    {
        delete m_strategy;
    }

    switch (format)
    {
    case NONE:
        qDebug() << "format of csv file not set";
        break;
    case RP5:
        m_strategy = new Rp5Strategy;
        Init(29, "Местное время", ';');
    default:
        break;
    }
}

CsvParser::t_lineStatus ClimateCsvParser::ParseLine(const QString& string, mt::t_observation &observation)
{
    t_lineStatus status = SplitLine(string);
    if (status != OK)
    {
        return status;
    }

    return m_strategy->ParseLine(m_list, observation);
}
