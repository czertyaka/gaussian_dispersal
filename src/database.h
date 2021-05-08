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
        bool operator<(const point& o) const;
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

    typedef struct coordSet
    {
        std::set<double> lat;
        std::set<double> lon;
    } t_coordSet;

    typedef struct srcTerrainCorrections
    {
        const mt::t_source& source;
        std::vector<double> data;
        srcTerrainCorrections(const mt::t_source& source) : source(source) {}
    } t_srcTerrainCorrections;
    typedef std::vector<t_srcTerrainCorrections> t_terrainCorrections;

    typedef struct distanceMask
    {
        const mt::t_source& source;
        std::vector<double> distances;
        std::vector<bool> mask; ///< true if corresponding point is within model
        distanceMask(const mt::t_source& source) : source(source) {}
    } t_distanceMask;

    typedef std::vector<t_distanceMask> t_distanceMasks;

    static DataBase& GetInstance();
    ~DataBase();

    // initialization
    bool Init();
    bool InitNuclides();

    // accessors
    t_climateJournal&       ClimateJournal();
    t_landscape&            Landscape();
    t_image&                Image();
    t_sources&              Sources();
    mt::t_matrix&           Matrix();
    t_nuclides&             Nuclides();
    t_coordSet&             CoordSet();
    t_terrainCorrections&   TerrainCorrections();
    t_distanceMasks&        Distances();

    // savers
    bool SaveMatrix(const QString& directory);
    bool SaveCorrections(const QString& directory);

private:
    DataBase();
    t_climateJournal*       m_climateJournal; ///< журнал климатических измерений
    t_landscape*            m_landscape; ///< массив точек с высотами и типом местности
    t_image*                m_image; ///< изображение подложки
    t_sources*              m_sources; ///< источники выбросов
    mt::t_matrix*           m_matrix; ///< матрица повторяемости и другие климатические параметры
    t_nuclides*             m_nuclides; ///< список еуклидов, считанных из файла и используемых в программе
    t_coordSet*             m_coordSet; ///< набор координат прямоугольного массива, используется для адресации вектора @ref m_landscape
    t_terrainCorrections    m_terrainCorrection; ///< поправки на рельеф
    t_distanceMasks         m_distanceMasks; ///< массивы расстояний до источника
};

typedef DataBase db;

#endif // DATABASEMANAGER_H
