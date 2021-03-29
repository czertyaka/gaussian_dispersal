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
    CsvParser(const size_t columnsNumber, const QString& firstHeader);
protected:
    t_lineStatus ParseLine(const QString& string);
    QStringList m_list;
private:
    int m_columnsNumber;
    QString m_firstHeader;
};

#endif // BASEPARSER_H
