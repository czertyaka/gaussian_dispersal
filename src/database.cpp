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
        QString nuclideName;
        mt::t_nuclideInfo nuclideInfo;
        CsvParser::t_lineStatus lineStatus = parser.ParseLine(line, nuclideInfo, nuclideName);

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
            m_nuclidesTable.insert(std::make_pair(nuclideName, nuclideInfo));
            break;
        default:
            break;
        }
    }
    file.close();

    MY_LOG("read " << m_nuclidesTable.size() << " nuclides from " << filename);
    return m_nuclidesTable.size() != 0;
}

mt::t_matrix &DataBase::Matrix()
{
    return m_matrix;
}

DataBase::t_nuclidesTable& DataBase::Nuclides()
{
    return m_nuclidesTable;
}

DataBase::t_coordSet &DataBase::CoordSet()
{
    return m_coordSet;
}

DataBase::t_terrainCorrectionsTable &DataBase::TerrainCorrections()
{
    return m_terrainCorrection;
}

DataBase::t_distancesTable &DataBase::Distances()
{
    return m_distanceTable;
}

DataBase::t_emissionsTable &DataBase::Emissions()
{
    return m_emissionsTable;
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

        for (size_t n = 0; n < m_matrix.N; ++n)
        {
            writer.AddItem(m_matrix.windDirVals[n]);
            writer.AddItem(m_matrix.windRoseCold[n]);
            writer.AddItem(m_matrix.windRoseWarm[n]);
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

        for (size_t k = 0; k < m_matrix.K; ++k)
        {
            writer.AddItem(m_matrix.windSpeedVals[k]);
            writer.AddItem(m_matrix.windSpRepCold[k]);
            writer.AddItem(m_matrix.windSpRepWarm[k]);
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
        writer.AddItem(m_matrix.calmRepCold);
        writer.AddItem(m_matrix.calmRepWarm);

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

        for (size_t j = 0; j < m_matrix.J; ++j)
        {
            writer.AddItem(m_matrix.smithParamVals[j]);
            writer.AddItem(m_matrix.smithParamRepCold[j]);
            writer.AddItem(m_matrix.smithParamRepWarm[j]);
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

        for (size_t n = 0; n < m_matrix.N; ++n)
        {
            writer.AddItem(m_matrix.windDirVals[n]);
            writer.AddItem(m_matrix.avWindSpByCPCold[n]);
            writer.AddItem(m_matrix.avWindSpByCPWarm[n]);
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

        for (size_t j = 0; j < m_matrix.J; ++j)
        {
            writer.AddItem(m_matrix.smithParamVals[j]);
            writer.AddItem(m_matrix.avWindSpBySPCold[j]);
            writer.AddItem(m_matrix.avWindSpBySPWarm[j]);
        }

        MY_LOG("finished writing to " << filename);
    }

    return true;
}

bool DataBase::SaveCorrections(const QString &directory)
{
    size_t counter = 0;

    for (auto it = m_terrainCorrection.cbegin();
         it != m_terrainCorrection.cend(); ++it)
    {
        {
            ++counter;

            QString filename = directory + "/terrain-corrections-src-"
                    + QString::number(counter) + ".csv";
            CsvWriter writer(filename, 3);
            if (!writer.Init())
            {
                MY_LOG("error writing to " << filename);
                return false;
            }

            QString buff;
            QTextStream comment(&buff);
            comment << "Terrain corrections for source #" << counter <<
                       " with coordiantes " << m_sources.find(it->first)->second.coordinates;
            writer.AddComment(*comment.string());
            writer.AddItem("Latitude");
            writer.AddItem("Longitude");
            writer.AddItem("Terrain correction");

            for (size_t i = 0; i < it->second.size(); ++i)
            {
                writer.AddItem(m_landscape.at(i).coord.lat);
                writer.AddItem(m_landscape.at(i).coord.lon);
                writer.AddItem(it->second.at(i));
            }

            MY_LOG("finished writing to " << filename);
        }
    }

    counter = 0;

    for (auto iter = m_distanceTable.cbegin();
         iter != m_distanceTable.cend(); ++iter)
    {
        {
            ++counter;

            QString filename = directory + "/distance-mask-src-"
                    + QString::number(counter) + ".csv";
            CsvWriter writer(filename, 3);
            if (!writer.Init())
            {
                MY_LOG("error writing to " << filename);
                return false;
            }

            QString buff;
            QTextStream comment(&buff);
            comment << "Disatnce mask for source #" << counter <<
                       " with coordiantes " << m_sources.find(iter->first)->second.coordinates;
            writer.AddComment(*comment.string());
            writer.AddComment("1 if distance between source and point is less than 30 km, 0 if vice versa");
            writer.AddItem("Latitude");
            writer.AddItem("Longitude");
            writer.AddItem("Mask");

            for (size_t i = 0; i < iter->second.mask.size(); ++i)
            {
                writer.AddItem(m_landscape.at(i).coord.lat);
                writer.AddItem(m_landscape.at(i).coord.lon);
                writer.AddItem(static_cast<bool>(iter->second.mask.at(i)));
            }

            MY_LOG("finished writing to " << filename);
        }
    }

    return true;
}

DataBase::t_climateJournal& DataBase::ClimateJournal()
{
    return m_climateJournal;
}

DataBase::t_landscape& DataBase::Landscape()
{
    return m_landscape;
}

DataBase::t_image& DataBase::Image()
{
    return m_image;
}

DataBase::t_sourcesTable& DataBase::Sources()
{
    return m_sources;
}

DataBase::DataBase()
{
}
