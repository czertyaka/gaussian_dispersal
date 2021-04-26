#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>

#include "meteorology.h"
#include "geospatialdataloader.h"
#include "imageloader.h"
#include "sourcesloader.h"

class DataBase
{
public:
    typedef std::vector<mm::t_observation> t_climateJournal;
    typedef std::vector<GeospatialDataLoader::t_point> t_landscape;
    typedef ImageLoader::t_image t_image;
    typedef SourcesLoader::t_vSources t_sources;
    static DataBase& GetInstance();
    ~DataBase();

    // accessors
    t_climateJournal&   ClimateJournal();
    t_landscape&        Landscape();
    t_image&            Image();
    t_sources&          Sources();
    mm::t_matrix&       Matrix();

    // savers
    bool SaveMatrix(const QString& directory);

private:
    DataBase();
    t_climateJournal*   m_climateJournal;
    t_landscape*        m_landscape;
    t_image*            m_image;
    t_sources*          m_sources;
    mm::t_matrix*       m_matrix;
};

#endif // DATABASEMANAGER_H
