#include "datainterface.h"

/**
 * @brief DataInterface::DataInterface
 * @param parent
 */
DataInterface::DataInterface(QObject *parent)
    : QObject(parent)
    , m_climaticVariablesLoader(new ClimaticVariablesLoader)
    , m_geospatialDataLoader(new GeospatialDataLoader)
    , m_imageLoader(new ImageLoader)
    , m_sourcesLoader(new SourcesLoader)
    , m_matrixCalculator(new MatrixCalculator)
{

}

DataInterface &DataInterface::GetInstance()
{
    static DataInterface instance;
    return  instance;
}

DataInterface::~DataInterface()
{
    delete m_climaticVariablesLoader;
    delete m_geospatialDataLoader;
    delete m_imageLoader;
    delete m_sourcesLoader;
    delete m_matrixCalculator;
}

void DataInterface::AddLog(const QTextStream &stream)
{
    QString log = *(stream.string());
    emit AddLogSignal(log);
}

bool DataInterface::CheckStatus()
{
    return m_climaticVariablesLoader->CheckStatus() == BaseDataLoader::READY &&
           m_geospatialDataLoader->CheckStatus() == BaseDataLoader::READY &&
           m_imageLoader->CheckStatus() == BaseDataLoader::READY &&
           m_sourcesLoader->CheckStatus() == BaseDataLoader::READY;
}

bool DataInterface::AddClimaticJournal(const QString &filename, ClimateCsvParser::t_format format)
{
    bool result = m_climaticVariablesLoader->AddJournal(filename, format);
    emit UpdateClimateStatusLabel(result);
    return result;
}

void DataInterface::OnClimateReset()
{
    m_climaticVariablesLoader->Reset();
}

bool DataInterface::AddGeospatialData(const QString &filename)
{
    bool result = m_geospatialDataLoader->AddFromFile(filename);
    emit UpdateGeoStatusLabel(result);
    return result;
}

void DataInterface::OnGeospatialReset()
{
    m_geospatialDataLoader->Reset();
}

bool DataInterface::AddImage(const QString &filename, const ImageLoader::t_optBorders& optBorders)
{
    bool result = m_imageLoader->AddImage(filename, optBorders);
    emit UpdateImageStatusLabel(result);
    return result;
}

void DataInterface::OnImageReset()
{
    m_imageLoader->Reset();
}

bool DataInterface::AddSources(const SourcesLoader::t_vSources& vSources)
{
    bool result = m_sourcesLoader->AddSources(vSources);
    emit UpdateSourcesStatusLabel(result);
    return result;
}

void DataInterface::OnSourcesReset()
{
    m_sourcesLoader->Reset();
}

void DataInterface::OnStart()
{
    if (m_matrixCalculator->Execute() == BaseCalculator::OK)
    {
        MY_LOG("repeatability matrix calculation: done");
    }
    else
    {
        MY_LOG("repeatability matrix calculation: error");
        MY_LOG("aborting calculation");
        return;
    }
}