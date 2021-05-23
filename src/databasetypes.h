#ifndef DATABASETYPES_H
#define DATABASETYPES_H

#include "globaltypes.h"

#include <set>
#include <map>
#include <QImage>
#include <stdexcept>

namespace dbt
{

///< vector that can be treated as 2-D array via operator()
///< also reimplemented some other methods
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
    vectorAsArray& SetDims(const size_t xDim, const size_t yDim)
    {
        m_xDim = xDim;
        m_yDim = yDim;
        if (this->size() != xDim * yDim)
        {
            throw std::invalid_argument("x dim and y dim don't fit array size");
        }
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
    void clear()
    {
        m_xDim = 0;
        m_yDim = 0;
        this->std::vector<T>::clear();
    }
    size_t XDim() const { return m_xDim; }
    size_t YDim() const { return m_yDim; }
private:
    size_t m_xDim;
    size_t m_yDim;
};

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
    bool operator<(const point &o) const
    {
        return coord.lat != o.coord.lat ? coord.lat < o.coord.lat : coord.lon < o.coord.lon;
    }
} t_point;

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

///< id of unique source (e.g. tube)
typedef size_t t_sourceId;

///< map of sources with their id's
typedef std::unordered_map<t_sourceId, mt::t_source> t_sourcesTable;

///< set of nuclides known to program
typedef std::set<mt::t_nuclide> t_nuclidesSet;

///< set of coordinates correspomding to the landscape 2-D array
typedef struct coordSet
{
    std::set<double> lon;
    std::set<double> lat;
} t_coordSet;

///< terrain corrections array
typedef vectorAsArray<double> t_terrainCorrections;

///< map containing terrain corrections for the source id
typedef std::unordered_map<t_sourceId, t_terrainCorrections> t_terrainCorrectionsTable;

///< distances cache
typedef struct distances
{
    vectorAsArray<double> value;
    vectorAsArray<char> mask; ///< true if corresponding point is within model
} t_distances;

///< array of distances from source to point, some kind of cache
///< may not need it tho, if that's so, get rid of this shit
typedef std::unordered_map<t_sourceId, t_distances> t_distancesTable;

///< id of unique source & nuclide combination
typedef size_t t_srcNuclideId;

///< table of unique source & nuclide combination
typedef std::unordered_map<t_srcNuclideId, std::pair<t_sourceId, mt::t_nuclide>>
    t_srcNuclideTable;

///< array of concentrations for source & nuclide combination
typedef vectorAsArray<double> t_concentrations;

///< key here is hash of source & nuclide combination
typedef std::unordered_map<t_srcNuclideId, t_concentrations> t_concentrationsTable;

} // namespace dbt

#endif // DATABASETYPES_H