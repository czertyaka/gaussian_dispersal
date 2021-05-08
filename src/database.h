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
        t_epsg4326coord coord;
        t_microrelief microrelief;
        short int elevation;
    } t_point;
    typedef std::vector<t_point> t_landscape;

    typedef struct borders
    {
        t_pseudoMercatorCoord nw; // nord west
        t_pseudoMercatorCoord ne; // nord east
        t_pseudoMercatorCoord sw; // south west
        t_pseudoMercatorCoord se; // south east
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
        std::set<double> x;
        std::set<double> y;
    } t_coordSet;

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
    t_coordSet&         CoordSet();

    // savers
    bool SaveMatrix(const QString& directory);

private:
    DataBase();
    t_climateJournal*   m_climateJournal; ///< журнал климатических измерений
    t_landscape*        m_landscape; ///< массив точек с высотами и типом местности
    t_image*            m_image; ///< изображение подложки
    t_sources*          m_sources; ///< источники выбросов
    mt::t_matrix*       m_matrix; ///< матрица повторяемости и другие климатические параметры
    t_nuclides*         m_nuclides; ///< список еуклидов, считанных из файла и используемых в программе
    t_coordSet*         m_coordSet; ///< набор координат прямоугольного массива, используется для адресации вектора @ref m_landscape
};

typedef DataBase db;

#endif // DATABASEMANAGER_H
