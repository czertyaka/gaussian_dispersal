#include "rp5csvparser.h"

#include <QStringList>

Rp5CsvParser::Rp5CsvParser()
{

}

bool Rp5CsvParser::Parse(const QString &string, mm::t_observation &observation)
{   
    if (string.at(0) == '#')
    {
        return false;
    }

    QString myString = string;
    myString.remove('"');

    QStringList list = myString.split(";");

    bool result = true;

    return result;
}
