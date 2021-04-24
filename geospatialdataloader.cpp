#include "geospatialdataloader.h"
#include "datainterface.h"
#include "geospatialcsvparser.h"
#include "database.h"

#include <QFile>

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

    DataBase::t_landscape* landscape = m_db.GetLandscape();
    if (!CheckPointer(landscape, "error opening geospatial database"))
    {
        return false;
    }

    landscape->clear();

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
            t_point point;
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
                landscape->push_back(point);
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
    }
    file.close();
    MY_LOG("geospatial data read successfully");
    m_status = READY;
    return true;
}

void GeospatialDataLoader::Reset()
{
    DataBase::t_landscape* landscape = m_db.GetLandscape();
    if(CheckPointer(landscape, "error opening geospatial database"))
    {
        landscape->clear();
        m_status = NOT_READY;
    }
}
