#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <QString>
#include <QFile>
#include <QTextStream>

#include "datainterface.h"

class CsvWriter
{
public:
    CsvWriter(const QString& filename, const size_t cols = 1,
              const char delimeter = ';', bool qoutes = true);
    ~CsvWriter();
    bool Init();

    bool AddComment(const QString& comment);

    template <class T>
    bool AddItem(T item)
    {
        ++m_colsCounter;

        if (m_colsCounter > m_cols)
        {
            MY_LOG("can not add an item: table row finished");
            return false;
        }

        if (m_colsCounter && m_colsCounter != m_cols)
        {
            (*m_stream) << m_delimeter;
        }

        if (m_quotes)
        {
            (*m_stream) << '"';
        }

        (*m_stream) << item;

        if (m_quotes)
        {
            (*m_stream) << '"';
        }
    };

    bool EOL();
private:
    QFile m_file;
    QTextStream* m_stream;

    const QString m_filename;
    const size_t m_cols; ///< columns number
    const char m_delimeter;
    const bool m_quotes; ///< flag if quotes warpping needed

    size_t m_colsCounter; ///< columns counter
};

#endif // CSVWRITER_H
