#include "csvwriter.h"

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
        return false;
    }

    m_stream = new QTextStream(&m_file);
    return true;
}

bool CsvWriter::AddComment(const QString &comment)
{
    if (m_colsCounter)
    {
        MY_LOG(" can not add a comment: table row not finished");
        return false;
    }

    (*m_stream) << "# " << comment << "\n";
    return true;
}

bool CsvWriter::EOL()
{
    if (m_colsCounter < m_cols)
    {
        MY_LOG(" can not add new line: table row not finished");
        return false;
    }

    m_colsCounter = 0;
    (*m_stream) << "\n";
    return true;
}

