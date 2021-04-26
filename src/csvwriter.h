#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <QString>
#include <QFile>
#include <QTextStream>

class CsvWriter
{
public:
    CsvWriter(const QString& filename, const size_t cols = 1,
              const char delimeter = ';', bool qoutes = true);
    ~CsvWriter();
    bool Init();

    void AddComment(const QString& comment);

    template <class T>
    void AddItem(T item)
    {
        if (m_colsCounter >= m_cols)
        {
            EOL();
        }

        if (m_colsCounter)
        {
            (*m_stream) << m_delimeter;
        }

        if (m_quotes)
        {
            (*m_stream) << '"' << item << '"';
        }
        else
        {
            (*m_stream) << item;
        }

        ++m_colsCounter;
    };

    void EOL();
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
