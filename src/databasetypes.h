#ifndef DATABASETYPES_H
#define DATABASETYPES_H

#include "globaltypes.h"
#include <set>
#include <QImage>

namespace dbt
{

typedef mt::t_matrix t_matrix;

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
    bool operator<(const point& o) const;
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
    std::set<double> lon;
    std::set<double> lat;
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

} // namespace dbt

#endif // DATABASETYPES_H
