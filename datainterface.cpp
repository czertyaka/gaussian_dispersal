#include "datainterface.h"

/**
 * @brief DataInterface::DataInterface
 * @param parent
 */
DataInterface::DataInterface(QObject *parent)
    : QObject(parent)
    , m_climaticVariables(new ClimaticVariables)
    , m_GeospatialData(new GeospatialData)
    , m_SourcesData(new SourcesData)
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

bool DataInterface::AddClimaticJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    bool result = m_climaticVariables->AddJournal(filename, format);
    emit UpdateClimateStatusLabel(result);
    return result;
}

bool DataInterface::AddGeospatialData(const QString &filename)
{
    bool result = m_GeospatialData->AddFromFile(filename);
    emit UpdateGeoStatusLabel(result);
    return result;
}

bool DataInterface::AddSources(const SourcesData::t_vSources& vSources)
{
    bool result = m_SourcesData->AddSources(vSources);
    emit UpdateSourcesStatusLabel(result);
    return result;
}
