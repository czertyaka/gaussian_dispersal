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
    t_climateJournal& GetClimateJournal();
    t_landscape& GetLandscape();
    t_image& GetImage();
    t_sources& GetSources();
    mm::t_matrix& Matrix();
private:
    DataBase();
    t_climateJournal*   m_climateJournal;
    t_landscape*        m_landscape;
    t_image*            m_image;
    t_sources*          m_sources;
    mm::t_matrix*       m_matrix;
};

#endif // DATABASEMANAGER_H
