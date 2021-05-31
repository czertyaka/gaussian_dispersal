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
    return InitNuclides() &&
           InitConstData();
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

bool DataBase::InitConstData()
{
    m_maxDiffusionParameter = {1600, 1200, 800, 400, 250, 200, 160}; // meters

    m_roughness = {1, 1, 4, 10, 40, 100, 1}; // cm;

    m_gCoeffs.a1 = {0.112, 0.130, 0.112, 0.098, 0.080, 0.0609, 0.0638};
    m_gCoeffs.a2 = {5.38e-4, 6.52e-4, 9.05e-4, 1.35e-3, 1.58e-3, 1.96e-3, 1.36e-3};
    m_gCoeffs.b1 = {1.06, 0.950, 0.920, 0.889, 0.892, 0.895, 0.783};
    m_gCoeffs.b2 = {0.815, 0.755, 0.718, 0.688, 0.686, 0.684, 0.672};
    // unitless

    m_fCoeffs.c1 = {1.56, 1.56, 1.56, 2.02, 2.72, 5.16, 7.37};
    m_fCoeffs.c2 = {6.25e-4, 6.25e-4, 6.25e-4, 7.76e-4, 0, 18.6, 4290};
    m_fCoeffs.d1 = {0.048, 0.048, 0.048, 0.0269, 0, -0.098, -0.0957};
    m_fCoeffs.d2 = {0.45, 0.45, 0.45, 0.37, 0, -0.255, -0.60};
    // unitless

    m_epsilon.e[mt::SP_A] = {0.05, 0.05, 0.05, 0.06, 0.08, 0.15, 0.16};
    m_epsilon.e[mt::SP_B] = {0.06, 0.06, 0.06, 0.07, 0.09, 0.16, 0.17};
    m_epsilon.e[mt::SP_C] = {0.06, 0.06, 0.06, 0.08, 0.11, 0.14, 0.20};
    m_epsilon.e[mt::SP_D] = {0.12, 0.12, 0.12, 0.13, 0.16, 0.20, 0.27};
    m_epsilon.e[mt::SP_E] = {0.22, 0.22, 0.22, 0.22, 0.22, 0.25, 0.31};
    m_epsilon.e[mt::SP_F] = {0.34, 0.34, 0.34, 0.34, 0.34, 0.35, 0.42};
    m_epsilon.e[mt::SP_G] = {0.52, 0.52, 0.52, 0.52, 0.52, 0.55, 0.60};
    // unitless

    return true;;
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

DataBase::t_dilutionFactorsTable& DataBase::Dilutions()
{
    return m_dilutionFactorsTable;
}

DataBase::t_concentrationsTable& DataBase::Concentrations()
{
    return m_concentrationsTable;
}

const DataBase::t_diffusionParameter &DataBase::MaxDiffusionPatameters()
{
    return m_maxDiffusionParameter;
}

const DataBase::t_roughness &DataBase::Roughness()
{
    return m_roughness;
}

const DataBase::t_gCoeffs &DataBase::GCoeffs()
{
    return m_gCoeffs;
}

const DataBase::t_fCoeffs &DataBase::FCoeffs()
{
    return m_fCoeffs;
}

const DataBase::t_epsilon &DataBase::Epsilon()
{
    return m_epsilon;
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

            QString filename = directory + "/distances-src-"
                    + QString::number(counter) + ".csv";
            CsvWriter writer(filename, 4);
            if (!writer.Init())
            {
                MY_LOG("error writing to " << filename);
                return false;
            }

            QString buff;
            QTextStream comment(&buff);
            comment << "Disatnces for source #" << counter <<
                       " with coordiantes " << m_sources.find(iter->first)->second.coordinates;
            writer.AddComment(*comment.string());
            writer.AddComment("1 if distance between source and point is less than 30 km, 0 if vice versa");
            writer.AddItem("Latitude");
            writer.AddItem("Longitude");
            writer.AddItem("Distance, m");
            writer.AddItem("Mask");

            for (size_t i = 0; i < iter->second.size(); ++i)
            {
                writer.AddItem(m_landscape.at(i).coord.lat);
                writer.AddItem(m_landscape.at(i).coord.lon);
                writer.AddItem(iter->second.at(i));
                writer.AddItem(iter->second.at(i) < gaussian_model_limit);
            }

            MY_LOG("finished writing to " << filename);
        }
    }

    return true;
}

bool DataBase::SaveDilutions(const QString &directory)
{
    std::multimap<mt::t_nuclideName, mt::t_emissionId> map;
    std::set<mt::t_nuclideName> nuclides;

    // init map
    for (auto emIt = m_emissionsTable.begin(); emIt != m_emissionsTable.end(); ++emIt)
    {
        map.insert(std::make_pair(emIt->second.nuclideName, emIt->first));
        nuclides.insert(emIt->second.nuclideName);
    }

    // save for each nuclide
    for (auto nuclide = nuclides.begin(); nuclide != nuclides.end(); ++nuclide)
    {
        auto range = map.equal_range(*nuclide);

        std::vector<const mt::t_dilutionFactors*> dilutions;
        std::vector<const mt::t_concentrations*> concentrations;
        for (auto it = range.first; it != range.second; ++it)
        {
            dilutions.push_back( &(m_dilutionFactorsTable.find(it->second)->second) );
            concentrations.push_back( &(m_concentrationsTable.find(it->second)->second) );
        }

        // summary
        QString filename = directory + "/dilutions-and-concentrations-" + *nuclide + "-sum.csv";
        CsvWriter writer(filename, 4);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Summary dilution factors and concentrations");
        writer.AddItem("Latitude");
        writer.AddItem("Longitude");
        writer.AddItem("Dilution factor, s/m^3");
        writer.AddItem("Concentration, Bq/m^3");

        for (size_t i = 0; i < m_landscape.size(); ++i)
        {
            writer.AddItem(m_landscape.at(i).coord.lat);
            writer.AddItem(m_landscape.at(i).coord.lon);
            double sumDilut = 0;
            for (auto emIt = dilutions.begin(); emIt != dilutions.end(); ++emIt)
            {
                sumDilut += (*emIt)->at(i);
            }
            double sumConc = 0;
            for (auto concIt = concentrations.begin(); concIt != concentrations.end(); ++concIt)
            {
                sumConc += (*concIt)->at(i);
            }
            writer.AddItem(sumDilut);
            writer.AddItem(sumConc);
        }

        MY_LOG("finished writing to " << filename);

        // by sources
        size_t srcCounter = 0;
        if (std::distance(range.first, range.second) > 1)
        {
            for (auto it = range.first; it != range.second; ++it)
            {
                srcCounter++;
                QString srcFilename = directory + "/dilutions-and-concentrations-" + *nuclide +
                        "-" + QString::number(srcCounter) + "-sum.csv";
                CsvWriter writer(srcFilename, 4);
                if (!writer.Init())
                {
                    MY_LOG("error writing to " << filename);
                    return false;
                }

                QString buff;
                QTextStream comment(&buff);
                const mt::t_source& src = m_sources.find( m_emissionsTable.find(it->second)->second.source )->second;
                comment << "Dilution factors and concentrations for source #" << srcCounter <<
                           " with coordiantes " << src.coordinates;
                writer.AddItem("Latitude");
                writer.AddItem("Longitude");
                writer.AddItem("Dilution factor, s/m^3");
                writer.AddItem("Concentration, Bq/m^3");

                const mt::t_dilutionFactors& dilution = m_dilutionFactorsTable.find(it->second)->second;
                const mt::t_concentrations& concentration = m_concentrationsTable.find(it->second)->second;

                for (size_t i = 0; i < m_landscape.size(); ++i)
                {
                    writer.AddItem(m_landscape.at(i).coord.lat);
                    writer.AddItem(m_landscape.at(i).coord.lon);
                    writer.AddItem(dilution.at(i));
                    writer.AddItem(concentration.at(i));
                }

                MY_LOG("finished writing to " << srcFilename);
            }
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

DataBase::DataBase() :
    m_concentrationsTable(m_dilutionFactorsTable)
{
}
