#include "csvwriter.h"

#ifndef TESTING
#include "datainterface.h"
#else
#define MY_LOG( X )
#endif // TESTING

CsvWriter::CsvWriter(const QString &filename, const size_t cols, const char delimeter, bool quotes) :
    m_stream(nullptr),
    m_filename(filename),
    m_cols(cols),
    m_delimeter(delimeter),
    m_quotes(quotes),
    m_colsCounter(0)
{

}

CsvWriter::~CsvWriter()
{
    if (m_stream)
    {
        delete m_stream;
    }

    m_file.close();
}

bool CsvWriter::Init()
{
    m_file.setFileName(m_filename);
    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        MY_LOG("error creating output file " << m_filename);
        MY_LOG(m_file.errorString());
        return false;
    }

    m_stream = new QTextStream(&m_file);
    return true;
}

void CsvWriter::AddComment(const QString &comment)
{
    if (m_colsCounter)
    {
        EOL();
    }
    (*m_stream) << "# " << comment;
    EOL();
}

void CsvWriter::EOL()
{
    while (m_colsCounter && m_colsCounter < m_cols)
    {
        (*m_stream) << m_delimeter;
        if (m_quotes)
        {
            (*m_stream) << "\"\"";
        }
        ++m_colsCounter;
    }

    (*m_stream) << "\n";
    m_colsCounter = 0;
}

