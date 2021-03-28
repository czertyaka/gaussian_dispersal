#include "datainterface.h"

/**
 * @brief DataInterface::DataInterface
 * @param parent
 */
DataInterface::DataInterface(QObject *parent)
    : QObject(parent)
    , m_climaticVariables(new ClimaticVariables())
    , m_GeospatialData(new GeospatialData())
{

}

DataInterface &DataInterface::GetInstance()
{
    static DataInterface instance;
    return  instance;
}

void DataInterface::AddLog(const QTextStream &stream)
{
    QString log = *(stream.string());
    emit AddLogSignal(log);
}

void DataInterface::AddClimaticJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    m_climaticVariables->AddJournal(filename, format);
}

void DataInterface::AddGeospatialData(const QString &filename)
{
    m_GeospatialData->AddFromFile(filename);
}
