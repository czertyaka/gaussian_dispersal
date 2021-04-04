#include "datainterface.h"

/**
 * @brief DataInterface::DataInterface
 * @param parent
 */
DataInterface::DataInterface(QObject *parent)
    : QObject(parent)
    , m_climaticVariables(new ClimaticVariables)
    , m_geospatialData(new GeospatialData)
    , m_imageData(new ImageData)
    , m_sourcesData(new SourcesData)
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

void DataInterface::OnClimateReset()
{
    m_climaticVariables->Reset();
}

bool DataInterface::AddGeospatialData(const QString &filename)
{
    bool result = m_geospatialData->AddFromFile(filename);
    emit UpdateGeoStatusLabel(result);
    return result;
}

void DataInterface::OnGeospatialReset()
{
    m_geospatialData->Reset();
}

bool DataInterface::AddImage(const QString &filename, const ImageData::t_optBorders& optBorders)
{
    bool result = m_imageData->AddImage(filename, optBorders);
    emit UpdateImageStatusLabel(result);
    return result;
}

void DataInterface::OnImageReset()
{

}

bool DataInterface::AddSources(const SourcesData::t_vSources& vSources)
{
    bool result = m_sourcesData->AddSources(vSources);
    emit UpdateSourcesStatusLabel(result);
    return result;
}

void DataInterface::OnSourcesReset()
{
    m_sourcesData->Reset();
}
