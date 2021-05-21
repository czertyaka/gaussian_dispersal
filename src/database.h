#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <QImage>
#include <optional>
#include <set>

#include "databasetypes.h"

class DataBase
{
public:

    typedef dbt::t_climateJournal t_climateJournal;
    typedef dbt::t_landscape t_landscape;
    typedef dbt::t_image t_image;
    typedef dbt::t_sources t_sources;
    typedef dbt::t_matrix t_matrix;
    typedef dbt::t_nuclides t_nuclides;
    typedef dbt::t_coordSet t_coordSet;
    typedef dbt::t_terrainCorrections t_terrainCorrections;
    typedef dbt::t_distanceMasks t_distanceMasks;

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
    t_matrix&           Matrix();
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
    t_matrix*               m_matrix; ///< матрица повторяемости и другие климатические параметры
    t_nuclides*             m_nuclides; ///< список нуклидов, считанных из файла и используемых в программе
    t_coordSet*             m_coordSet; ///< набор координат прямоугольного массива, используется для адресации вектора @ref m_landscape
    t_terrainCorrections    m_terrainCorrection; ///< поправки на рельеф
    t_distanceMasks         m_distanceMasks; ///< массивы расстояний до источника
};

typedef DataBase db;

#endif // DATABASEMANAGER_H
