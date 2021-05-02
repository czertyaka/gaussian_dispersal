#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <QImage>
#include <optional>
#include <set>

#include "globaltypes.h"

class DataBase
{
public:
    typedef std::vector<mt::t_observation> t_climateJournal;

    enum t_microrelief : unsigned short
    {
        UNKNOWN,
        SNOW,
        SHORTGRASS,
        TALLGRASS,
        SCRUB_GROWTH,
        FOREST,
        BUILDINGS,
        WATER
    };
    typedef struct point
    {
        mt::t_epsg4326coord coord;
        t_microrelief microrelief;
        short int elevation;
    } t_point;
    typedef std::vector<t_point> t_landscape;

    typedef struct borders
    {
        mt::t_pseudoMercatorCoord nw; // nord west
        mt::t_pseudoMercatorCoord ne; // nord east
        mt::t_pseudoMercatorCoord sw; // south west
        mt::t_pseudoMercatorCoord se; // south east
    } t_borders;
    typedef std::optional<t_borders> t_optBorders;
    typedef struct image
    {
        QImage picture;
        t_optBorders borders;
    } t_image;

    typedef std::vector<mt::t_source> t_sources;

    typedef std::set<mt::t_nuclide> t_nuclides;

    static DataBase& GetInstance();
    ~DataBase();

    // initialization
    bool Init();
    bool InitNuclides();

    // accessors
    t_climateJournal&   ClimateJournal();
    t_landscape&        Landscape();
    t_image&            Image();
    t_sources&          Sources();
    mt::t_matrix&       Matrix();
    t_nuclides&         Nuclides();

    // savers
    bool SaveMatrix(const QString& directory);

private:
    DataBase();
    t_climateJournal*   m_climateJournal;
    t_landscape*        m_landscape;
    t_image*            m_image;
    t_sources*          m_sources;
    mt::t_matrix*       m_matrix;
    t_nuclides*         m_nuclides;
};

typedef DataBase db;

#endif // DATABASEMANAGER_H
