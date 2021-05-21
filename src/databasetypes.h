#ifndef DATABASETYPES_H
#define DATABASETYPES_H

#include "globaltypes.h"

#include <set>
#include <map>
#include <QImage>

namespace dbt
{

///< repeatability matrix (joint frequency)
typedef mt::t_matrix t_matrix;

///< long term meteorological information for the site of interest
typedef std::vector<mt::t_observation> t_climateJournal;

///< microrelief types
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

///< struct for geospatial point info
typedef struct point
{
    t_epsg4326coord coord;
    t_microrelief microrelief;
    short int elevation;
    bool operator<(const point& o) const;
} t_point;

///< vector that can be treated as 2-D array via operator()
template <typename T>
class vectorAsArray : public std::vector<T>
{
public:
    vectorAsArray() : std::vector<T>(), m_xDim(0) {}
    vectorAsArray& Init(const size_t xDim, const size_t yDim)
    {
        m_xDim = xDim;
        m_yDim = xDim;
        this->resize(xDim * yDim);
        return *this;
    }
    template<typename M>
    vectorAsArray& Init(const vectorAsArray<M>& o)
    {
        m_xDim = o.XDim();
        m_yDim = o.YDim();
        this->resize(m_xDim * m_yDim);
        return *this;
    }
    T& at(const size_t x, const size_t y)
    {
        return this->std::vector<T>::at(y * m_xDim + x);
    }
    T& at(const size_t i)
    {
        return this->std::vector<T>::at(i);
    }
    const T& at(const size_t x, const size_t y) const
    {
        return this->std::vector<T>::at(y * m_xDim + x);
    }
    const T& at(const size_t i) const
    {
        return this->std::vector<T>::at(i);
    }
    size_t XDim() const { return m_xDim; }
    size_t YDim() const { return m_yDim; }
private:
    size_t m_xDim;
    size_t m_yDim;
};

///< landscape container
typedef vectorAsArray<t_point> t_landscape;

///< coordinates of image borders
typedef struct borders
{
    t_pseudoMercatorCoord nw; // nord west
    t_pseudoMercatorCoord ne; // nord east
    t_pseudoMercatorCoord sw; // south west
    t_pseudoMercatorCoord se; // south east
} t_borders;

///< we don't necessary need them tho...
typedef std::optional<t_borders> t_optBorders;

///< struct to store image info
typedef struct image
{
    QImage picture;
    t_optBorders borders;
} t_image;

///< map of sources with their id's
typedef std::unordered_map<size_t, mt::t_source> t_sources;

///< set of nuclides known to program
typedef std::set<mt::t_nuclide> t_nuclides;

///< set of coordinates correspomding to the landscape 2-D array
typedef struct coordSet
{
    std::set<double> lon;
    std::set<double> lat;
} t_coordSet;

///< terrain corrections array
typedef vectorAsArray<double> t_srcTerrainCorrections;

///< map containing terrain corrections for the source id
typedef std::unordered_map<size_t, t_srcTerrainCorrections> t_terrainCorrections;

///< distances cache
typedef struct distanceMask
{
    vectorAsArray<double> distances;
    vectorAsArray<char> mask; ///< true if corresponding point is within model
} t_distanceMask;

typedef std::unordered_map<size_t, t_distanceMask> t_distanceMasks;

} // namespace dbt

#endif // DATABASETYPES_H
