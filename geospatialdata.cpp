#include "geospatialdata.h"
#include "datainterface.h"
#include "geospatialcsvparser.h"
#include "databasemanager.h"

#include <QFile>

GeospatialData::GeospatialData()
    : m_parser(new GeospatialCsvParser())
{
    if (!m_parser)
    {
        MY_LOG(__PRETTY_FUNCTION__ << ": error creating geospatial data parser");
    }

    m_status = ERROR;
}

GeospatialData::~GeospatialData()
{
    delete m_parser;
}

void GeospatialData::AddFromFile(const QString &filename)
{
    DataBaseManager::t_landscape landscape = m_dbManager.GetLandscape();

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        MY_LOG( __PRETTY_FUNCTION__ << ": error opening file");
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

            if (lineStatus == CsvParser::NOT_A_DATA)
            {
                continue;
            }
            else if (lineStatus == CsvParser::INVALID)
            {
                MY_LOG(__PRETTY_FUNCTION__ << ": error reading line \""
                       << line << "\"");
                m_status = ERROR;
                return;
            }
            else if (lineStatus == CsvParser::OK)
            {
                landscape.push_back(point);
                pointsCounter++;
            }
        }
        MY_LOG(__PRETTY_FUNCTION__ << ": added " << pointsCounter
               << " geospatial points");

        if (!pointsCounter)
        {
            m_status = ERROR;
            return;
        }
    }
    file.close();
    MY_LOG(__PRETTY_FUNCTION__ << ": geospatial data read successfully");
    m_status = READY;
}
