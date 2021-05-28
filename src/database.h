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

    typedef mt::t_climateJournal            t_climateJournal;
    typedef mt::t_landscape                 t_landscape;
    typedef mt::t_image                     t_image;
    typedef mt::t_sourcesTable              t_sourcesTable;
    typedef mt::t_matrix                    t_matrix;
    typedef mt::t_nuclidesTable             t_nuclidesTable;
    typedef mt::t_coordSet                  t_coordSet;
    typedef mt::t_terrainCorrectionsTable   t_terrainCorrectionsTable;
    typedef mt::t_distancesTable            t_distancesTable;
    typedef mt::t_emissionsTable            t_emissionsTable;
    typedef mt::t_dilutionFactorsTable      t_dilutionFactorsTable;
    typedef mt::t_concentrationsTable       t_concentrationsTable;
    typedef mt::t_diffusionParameter        t_diffusionParameter;
    typedef mt::t_roughness                 t_roughness;
    typedef mt::t_gCoeffs                   t_gCoeffs;
    typedef mt::t_fCoeffs                   t_fCoeffs;
    typedef mt::t_epsilon                   t_epsilon;

    static DataBase& GetInstance();
    ~DataBase();

    // initialization
    bool Init();

    // accessors
    t_climateJournal&           ClimateJournal();
    t_landscape&                Landscape();
    t_image&                    Image();
    t_sourcesTable&             Sources();
    t_matrix&                   Matrix();
    t_nuclidesTable&            Nuclides();
    t_coordSet&                 CoordSet();
    t_terrainCorrectionsTable&  TerrainCorrections();
    t_distancesTable&           Distances();
    t_emissionsTable&           Emissions();
    t_dilutionFactorsTable&     Dilutions();
    t_concentrationsTable&      Concentrations();

    // const accessors (reference data)
    const t_diffusionParameter& MaxDiffusionPatameters();
    const t_roughness&          Roughness();
    const t_gCoeffs&            GCoeffs();
    const t_fCoeffs&            FCoeffs();
    const t_epsilon&            Epsilon();

    // savers
    bool SaveMatrix(const QString& directory);
    bool SaveCorrections(const QString& directory);

private:
    DataBase();
    bool InitNuclides();
    bool InitConstData();

    t_climateJournal            m_climateJournal; ///< журнал климатических измерений
    t_landscape                 m_landscape; ///< массив точек с высотами и типом местности
    t_image                     m_image; ///< изображение подложки
    t_sourcesTable              m_sources; ///< источники выбросов
    t_matrix                    m_matrix; ///< матрица повторяемости и другие климатические параметры
    t_nuclidesTable             m_nuclidesTable; ///< список нуклидов, считанных из файла и используемых в программе
    t_coordSet                  m_coordSet; ///< набор координат прямоугольного массива, используется для адресации вектора @ref m_landscape
    t_terrainCorrectionsTable   m_terrainCorrection; ///< поправки на рельеф
    t_distancesTable            m_distanceTable; ///< массивы расстояний до источника
    t_emissionsTable            m_emissionsTable; ///< emissions table
    t_dilutionFactorsTable      m_dilutionFactorsTable; ///< dilution factors
    t_concentrationsTable       m_concentrationsTable; ///< conecntration values
    t_diffusionParameter        m_maxDiffusionParameter; ///< max diffusion parameters for smith parameter, m
    t_roughness                 m_roughness; ///< roughness heights, cm
    t_gCoeffs                   m_gCoeffs; ///< coefficinets for g(x)
    t_fCoeffs                   m_fCoeffs; ///< coefficients for f(z0, x)
    t_epsilon                   m_epsilon; ///< ε values for wind speed formula
};

typedef DataBase db;

#endif // DATABASEMANAGER_H
