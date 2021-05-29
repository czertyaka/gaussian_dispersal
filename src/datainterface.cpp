#include "datainterface.h"
#include "landscapecalculator.h"
#include "database.h"

/**
 * @brief DataInterface::DataInterface
 * @param parent
 */
DataInterface::DataInterface(QObject *parent) :
    QObject(parent),
    m_climaticVariablesLoader(new ClimaticVariablesLoader),
    m_geospatialDataLoader(new GeospatialDataLoader),
    m_imageLoader(new ImageLoader),
    m_sourcesLoader(new SourcesLoader),
    m_matrixCalculator(new MatrixCalculator),
    m_landscapeCalculator(new LandscapeCalculator),
    m_dilutionsCalculator(new DilutionsCalculator),
    m_database(DataBase::GetInstance())
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
    delete m_landscapeCalculator;
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

bool DataInterface::AddImage(const QString &filename, const mt::t_optBorders& optBorders)
{
    bool result = m_imageLoader->AddImage(filename, optBorders);
    emit UpdateImageStatusLabel(result);
    return result;
}

void DataInterface::OnImageReset()
{
    m_imageLoader->Reset();
}

bool DataInterface::AddSources(const mt::t_sourcesTable& sources, const mt::t_emissionsTable& emissions)
{
    bool result = m_sourcesLoader->AddSources(sources, emissions);
    emit UpdateSourcesStatusLabel(result);
    return result;
}

void DataInterface::OnSourcesReset()
{
    m_sourcesLoader->Reset();
}

void DataInterface::OnStart()
{
    // init source coordinates
    for (auto src = m_database.Sources().begin();
         src != m_database.Sources().end(); ++src)
    {
        src->second.InitCoordinates();
    }

    MY_LOG("repeatability matrix calculation: start")
    if (m_matrixCalculator->Execute() == BaseCalculator::OK)
    {
        MY_LOG("repeatability matrix calculation: done");
        emit MatrixDone(true);
    }
    else
    {
        MY_LOG("repeatability matrix calculation: error, "
               "aborting calculation");
        emit MatrixDone(false);
        return;
    }

    MY_LOG("terrain corrections calculation: start")
    if (m_landscapeCalculator->Execute() == BaseCalculator::OK)
    {
        MY_LOG("terrain corrections calculation: done");
        emit TerrainDone(true);
    }
    else
    {
        MY_LOG("terrain corrections calculation: error, "
               "aborting calculation");
        emit TerrainDone(false);
        return;
    }

    MY_LOG("dilution factors and concentration calculation: start");
    if (m_dilutionsCalculator->Execute() == BaseCalculator::OK)
    {
        MY_LOG("dilution factors and concentration calculation: done");
        emit DilutionsDone(true);
    }
    else
    {
        MY_LOG("dilution factors and concentration calculation: error, "
               "aborting calculation");
        emit DilutionsDone(false);
        return;
    }
}

void DataInterface::SaveClimate(const QString& directory)
{
    if (!m_database.SaveMatrix(directory))
    {
        MY_LOG("error saving repeatability matrix");
    }
}

void DataInterface::SaveTerrain(const QString &directory)
{
    if (!m_database.SaveCorrections(directory))
    {
        MY_LOG("error saving terrain corrections")
    }
}
