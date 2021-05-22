#include "geospatialdataloader.h"
#include "datainterface.h"
#include "geospatialcsvparser.h"
#include "database.h"

#include <QFile>
#include <cmath>

GeospatialDataLoader::GeospatialDataLoader()
    : m_parser(new GeospatialCsvParser())
{
    if (!m_parser)
    {
        MY_LOG("error creating geospatial data parser");
    }

    m_status = ERROR;
}

GeospatialDataLoader::~GeospatialDataLoader()
{
    delete m_parser;
}

bool GeospatialDataLoader::AddFromFile(const QString &filename)
{ 
    if (filename.isEmpty())
    {
        MY_LOG("geospatial data filename field is empty");
        return false;
    }

    dbt::t_landscape& landscape = m_db.Landscape();
    landscape.clear();

    std::set<dbt::t_point> landscapeSet;

    dbt::t_coordSet& coordSet = m_db.CoordSet();
    coordSet.lon.clear();
    coordSet.lat.clear();

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        MY_LOG( "error opening file");
        return false;
    }
    else
    {
        size_t pointsCounter = 0;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            dbt::t_point point;
            CsvParser::t_lineStatus lineStatus = m_parser->ParseLine(line, point);

            if (lineStatus == CsvParser::COLUMNS_MISMATCH)
            {
                MY_LOG("geospatial file must consist at least of 5 columns");
                m_status = ERROR;
                return false;
            }

            if (lineStatus == CsvParser::NOT_A_DATA)
            {
                continue;
            }
            else if (lineStatus == CsvParser::INVALID)
            {
                MY_LOG("error reading line \""
                       << line << "\"");
                continue;
            }
            else if (lineStatus == CsvParser::OK)
            {
                landscapeSet.insert(point);
                coordSet.lon.insert(point.coord.lon);
                coordSet.lat.insert(point.coord.lat);
                pointsCounter++;
            }
        }

        for (auto it = landscapeSet.cbegin(); it != landscapeSet.cend(); ++it)
        {
            landscape.push_back(*it);
        }

        MY_LOG("added " << pointsCounter << " (" << coordSet.lat.size()
               << "x" << coordSet.lon.size() << ") geospatial points");

        if (!pointsCounter)
        {
            m_status = ERROR;
            return false;
        }

        try
        {
            landscape.SetDims(coordSet.lon.size(), coordSet.lat.size());
        }
        catch (std::invalid_argument& ex)
        {
            MY_LOG("points in " << filename << " do not provide regular rectangular grid: "
                   "found " << coordSet.lon.size() << "x" << coordSet.lat.size() << " unique "
                   "coordinates with " << landscape.size() << " total points");
            MY_LOG(ex.what());
            m_status = ERROR;
            return false;
        }
    }
    file.close();
    MY_LOG("geospatial data read successfully");
    m_status = READY;
    return true;
}

void GeospatialDataLoader::Reset()
{
    dbt::t_landscape landscape = m_db.Landscape();
}
