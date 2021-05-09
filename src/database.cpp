#include "database.h"
#include "csvwriter.h"
#include "datainterface.h"
#include "nuclidesparser.h"
#include "geography.h"

#include <cassert>

DataBase &DataBase::GetInstance()
{
    static DataBase Instance;
    return Instance;
}

DataBase::~DataBase()
{
    delete m_climateJournal;
    delete m_landscape;
    delete m_image;
    delete m_sources;
    delete m_matrix;
    delete m_nuclides;
    delete m_coordSet;
}

bool DataBase::Init()
{
    return InitNuclides();
}

bool DataBase::InitNuclides()
{
    QString filename = "../data/nuclides.csv";
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        MY_LOG( "error opening " << filename);
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();

        NuclidesParser parser;
        mt::t_nuclide nuclide;
        CsvParser::t_lineStatus lineStatus = parser.ParseLine(line, nuclide);

        switch (lineStatus)
        {
        case CsvParser::NOT_A_DATA:
            continue;
            break;
        case CsvParser::INVALID:
        case CsvParser::COLUMNS_MISMATCH:
            MY_LOG("error reading " << filename);
            return false;
            break;
        case CsvParser::OK:
            m_nuclides->insert(nuclide);
            break;
        default:
            break;
        }
    }
    file.close();

    MY_LOG("read " << m_nuclides->size() << " nuclides from " << filename);
    return m_nuclides->size() != 0;
}

mt::t_matrix &DataBase::Matrix()
{
    assert(m_matrix);
    return *m_matrix;
}

DataBase::t_nuclides &DataBase::Nuclides()
{
    assert(m_nuclides);
    return *m_nuclides;
}

DataBase::t_coordSet &DataBase::CoordSet()
{
    assert(m_coordSet);
    return *m_coordSet;
}

DataBase::t_terrainCorrections &DataBase::TerrainCorrections()
{
    return m_terrainCorrection;
}

DataBase::t_distanceMasks &DataBase::Distances()
{
    return m_distanceMasks;
}

bool DataBase::SaveMatrix(const QString& directory)
{
    {
        QString filename = directory + "/wind-rose.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Wind rose");
        writer.AddItem("Compass point, deg.");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t n = 0; n < m_matrix->N; ++n)
        {
            writer.AddItem(m_matrix->windDirVals[n]);
            writer.AddItem(m_matrix->windRoseCold[n]);
            writer.AddItem(m_matrix->windRoseWarm[n]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/wind-speed-repetitions.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Absolute wind speed value repetitions");
        writer.AddItem("Wind speed, m/s");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t k = 0; k < m_matrix->K; ++k)
        {
            writer.AddItem(m_matrix->windSpeedVals[k]);
            writer.AddItem(m_matrix->windSpRepCold[k]);
            writer.AddItem(m_matrix->windSpRepWarm[k]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/calm-repetitions.csv";
        CsvWriter writer(filename, 2);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Calm repetitions");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");
        writer.AddItem(m_matrix->calmRepCold);
        writer.AddItem(m_matrix->calmRepWarm);

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/smith-param-repetitions.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Smith parameter values repetitions");
        writer.AddItem("Smith parameter");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t j = 0; j < m_matrix->J; ++j)
        {
            writer.AddItem(m_matrix->smithParamVals[j]);
            writer.AddItem(m_matrix->smithParamRepCold[j]);
            writer.AddItem(m_matrix->smithParamRepWarm[j]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/wind-speed-by-direction.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Wind speed values by direction, m/s");
        writer.AddItem("Compass point, deg.");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t n = 0; n < m_matrix->N; ++n)
        {
            writer.AddItem(m_matrix->windDirVals[n]);
            writer.AddItem(m_matrix->avWindSpByCPCold[n]);
            writer.AddItem(m_matrix->avWindSpByCPWarm[n]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/wind-speed-by-smith-param.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Wind speed values by direction, m/s");
        writer.AddItem("Smith param");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t j = 0; j < m_matrix->J; ++j)
        {
            writer.AddItem(m_matrix->smithParamVals[j]);
            writer.AddItem(m_matrix->avWindSpBySPCold[j]);
            writer.AddItem(m_matrix->avWindSpBySPWarm[j]);
        }

        MY_LOG("finished writing to " << filename);
    }

    return true;
}

bool DataBase::SaveCorrections(const QString &directory)
{
    for (auto srcIter = m_terrainCorrection.cbegin();
         srcIter != m_terrainCorrection.cend(); ++srcIter)
    {
        QString filename = directory + "/terrain-corrections-src-"
                            + QString::number(srcIter->source.id) + ".csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        QString buff;
        QTextStream comment(&buff);
        comment << "Terrain corrections for source #" << srcIter->source.id <<
                   " with coordiantes " << srcIter->source.coordinates;
        writer.AddComment(*comment.string());
        writer.AddItem("Latitude");
        writer.AddItem("Longitude");
        writer.AddItem("Terrain correction");

        for (size_t i = 0; i < srcIter->data.size(); ++i)
        {
            writer.AddItem(m_landscape->at(i).coord.lat);
            writer.AddItem(m_landscape->at(i).coord.lon);
            writer.AddItem(srcIter->data.at(i));
        }

        MY_LOG("finished writing to " << filename);
    }

    return true;
}

DataBase::t_climateJournal& DataBase::ClimateJournal()
{
    assert(m_climateJournal);
    return *m_climateJournal;
}

DataBase::t_landscape& DataBase::Landscape()
{
    assert(m_landscape);
    return *m_landscape;
}

DataBase::t_image& DataBase::Image()
{
    assert(m_image);
    return *m_image;
}

DataBase::t_sources& DataBase::Sources()
{
    assert(m_sources);
    return *m_sources;
}

DataBase::DataBase() :
    m_climateJournal(new t_climateJournal),
    m_landscape(new t_landscape),
    m_image(new t_image),
    m_sources(new t_sources),
    m_matrix(new mt::t_matrix),
    m_nuclides(new t_nuclides),
    m_coordSet(new t_coordSet)
{
}

bool DataBase::point::operator<(const DataBase::point &o) const
{
    return coord.lat != o.coord.lat ? coord.lat < o.coord.lat : coord.lon < o.coord.lon;
}
