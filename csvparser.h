#ifndef BASEPARSER_H
#define BASEPARSER_H

#include <QString>
#include <QStringList>

class CsvParser
{
public:
    enum t_lineStatus
    {
        OK,
        MISSING_DATA,
        INVALID,
        NOT_A_DATA,
        COLUMNS_MISMATCH
    };
    CsvParser(const size_t columnsNumber, const QString& firstHeader, const char delimeter);
protected:
    t_lineStatus ParseLine(const QString& string);
    QStringList m_list;
private:
    const int m_columnsNumber;
    const QString m_firstHeader;
    const char m_delimeter;
};

#endif // BASEPARSER_H
