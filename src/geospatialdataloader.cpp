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

    db::t_landscape& landscape = m_db.Landscape();
    landscape.clear();

    db::t_coordSet& coordSet = m_db.CoordSet();
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
            db::t_point point;
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
                landscape.push_back(point);
                coordSet.lon.insert(point.coord.lon);
                coordSet.lat.insert(point.coord.lat);
                pointsCounter++;
            }
        }

        MY_LOG("added " << pointsCounter
               << " geospatial points");

        if (!pointsCounter)
        {
            m_status = ERROR;
            return false;
        }

        if (landscape.size() != coordSet.lon.size() * coordSet.lat.size())
        {
            MY_LOG("points in " << filename << " do not provide regular rectangular grid: "
                   "found " << coordSet.lon.size() << "x" << coordSet.lat.size() << " unique "
                   "coordinates with " << landscape.size() << " total points");
            m_status = ERROR;
            return false;
        }

        mt::t_epsg3857coord lowerLeft = mt::t_epsg3857coord(mt::t_epsg4326coord(landscape.begin()->coord.lon, landscape.begin()->coord.lat));
        mt::t_epsg3857coord upperRight = mt::t_epsg3857coord(mt::t_epsg4326coord((landscape.end() - 1)->coord.lon, (landscape.end() - 1)->coord.lat));
        double maxDistance = sqrt(pow(upperRight.easting - lowerLeft.easting, 2) + pow(upperRight.northing - lowerLeft.northing, 2));
        if (maxDistance > 60000)
        {
            MY_LOG("maximum distance between points " << maxDistance << " km exceeds 2*30 km = 60 km");
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
    db::t_landscape landscape = m_db.Landscape();
}
