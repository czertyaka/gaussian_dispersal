#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

#include <QString>
#include <QDateTime>
#include <cmath>
#include <set>
#include <map>
#include <QImage>
#include <stdexcept>

#include "geography.h"

#ifndef DATETIME_FORMAT
#define DATETIME_FORMAT "MM-dd-yyyy hh:00:00"
#endif

#if !defined(RAD) && !defined(DEG)
#define RAD(degree) static_cast<double>(degree)/180*M_PI
#define DEG(radian) static_cast<double>(radian)/M_PI*180
#endif

namespace mt ///< my types
{
    ///< wind directions
    enum t_windDir : const size_t
    {
        N,
        NNW,
        NW,
        WNW,
        W,
        WSW,
        SW,
        SSW,
        S,
        SSE,
        SE,
        ESE,
        E,
        ENE,
        NE,
        NNE,
        WIND_DIR_COUNT,
        calm
    };

    t_windDir degree_to_dir(double degree);

    ///< athmosphere stability categories
    enum t_smithParam : size_t
    {
        SP_A,
        SP_B,
        SP_C,
        SP_D,
        SP_E,
        SP_F,
        SP_G,
        SP_COUNT
    };

    ///< diffusion parameter, m
    typedef std::array<double, mt::SP_COUNT> t_diffusionParameter;

    ///< coefficients for g(x)
    typedef struct gCoeffs
    {
        typedef std::array<double, SP_COUNT> t_vals;
        t_vals a1;
        t_vals a2;
        t_vals b1;
        t_vals b2;
    } t_gCoeffs;

    ///< obv, why not just use Qt here?
    enum month_t : const int
    {
        JAN = 1,
        FEB = 2,
        MAR = 3,
        APR = 4,
        MAY = 5,
        JUN = 6,
        JUL = 7,
        AUG = 8,
        SEP = 9,
        OCT = 10,
        NOV = 11,
        DEC = 12
    };

    ///< single climatic obserevation to ba added to journal
    typedef struct observation
    {
        QDateTime dateTime; ///< дата и время наблюдения

        t_windDir windDir; ///< направление ветра, румб
        double windSpeed; ///< скорость ветра, м/с

        int cloudAmount; ///< балл общей облачности, от 0 до 10
        int lowerCloudAmount; ///< балл нижней облачности, от 0 до 10
        bool fog; ///< наличие тумана
        bool snow; ///< наличие сплошного снежного покрова

        double temper; ///< температура, град. Цельсия
    } t_observation;

    class InterpolatedSlice
    {
    public:
        typedef std::array<double, WIND_DIR_COUNT> t_nodes;
        InterpolatedSlice();
        double operator()(double degree) const;
        void Init(t_nodes& nodes);
    private:
        typedef struct coeffs
        {
            double a;
            double b;
            double c;
            double d;
        } t_coeffs;
        t_coeffs m_coeffs[WIND_DIR_COUNT]; ///< interpolating coeffs
    };

    ///< joint frequency of wind direction sector n, stability category j and wind speed class k
    struct Matrix
    {
        static const int N = WIND_DIR_COUNT; ///< количество интервалов направления ветра
        static const int J = 7; ///< количество интервалов категорий устойчивости атмосферы
        static const int K = 8; ///< количество интервалов скорости ветра

        static constexpr double windDirVals[N] = {90, 112.5, 135, 157.5, 180, 202.5, 225,
                                       247.5, 270, 292.5, 315, 337.5, 0, 22.5, 45, 67.5};
        static constexpr char smithParamVals[J] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
        static constexpr double windSpeedVals[K] = { 0.5, 1, 2, 3, 4.5, 6.5, 9, 12};

        int mCold[N][J][K]; ///< ненормированная матрица холодного времени года
        int mWarm[N][J][K]; ///< ненормированная матрица теплого времени года
        double wCold[N][J][K]; ///< нормированная матрица холодного времени года
        double wWarm[N][J][K]; ///< нормированная матрица теплого времени года

        int MCold = 0; ///< количество наблюдений в холодное время года
        int MWarm = 0; ///< количество наблюдений в теплое время года
        int MColdNoCalm = 0; ///< количество наблюдений в холодное время года без штилей
        int MWarmNoCalm = 0; ///< количество наблюдений в теплое время года без штилей

        double windRoseCold[N] = {0}; ///< повт. напр. ветра n-го румба в холодное время года
        double windRoseWarm[N] = {0}; ///< повт. напр. ветра n-го румба в теплое время года

        double windSpRepByCPCold[N][K]; ///< повт. скор. ветра k по напр. n в холодное время года
        double windSpRepByCPWarm[N][K]; ///< повт. скор. ветра k по напр. n в теплое время года

        double windSpRepCold[K] = {0}; ///< повт. скор. ветра k в холодное время года
        double windSpRepWarm[K] = {0}; ///< повт. скор. ветра k в теплое время года

        double calmRepCold = 0; ///< повторяемость штилей в холодное время года
        double calmRepWarm = 0; ///< повторяемость штилей в теплое время года

        double smithParamRepCold[J] = {0}; ///< повторяемость категорий устойчивости в холодное время года
        double smithParamRepWarm[J] = {0}; ///< повторяемость категорий устойчивости в теплое время года

        double avWindSpByCPCold[N] = {0}; ///< ср. скорость ветра n-го румб в холодное время года
        double avWindSpByCPWarm[N] = {0}; ///< ср. скорость ветра n-го румб в теплое время года

        double avWindSpBySPCold[J] = {0}; ///< ср. скор. ветра при j−ой кат. уст. в холодное время года
        double avWindSpBySPWarm[J] = {0}; ///< ср. скор. ветра при j−ой кат. уст. в теплое время года

        InterpolatedSlice interpCold[J][K]; ///< object handling interpolated slice of matrix for cold season
        InterpolatedSlice interpWarm[J][K]; ///< object handling interpolated slice of matrix for warm season

        Matrix();
    };

    typedef Matrix t_matrix;

    ///< emission value, GBq
    typedef double t_emissionValue;

    ///< emession values by year quarter
    typedef struct quarterEmission
    {
        t_emissionValue fisrtQuarter;
        t_emissionValue secondQuarter;
        t_emissionValue thirdQuarter;
        t_emissionValue fourthQuarter;
    } t_quarterEmissionValue;

    ///< emission value to handle and amange annual and quarterly emissions value
    typedef class emissionValue
    {
    public:
        emissionValue();
        emissionValue(double annualValue);
        emissionValue(double fisrt, double second, double third, double fourth);
        emissionValue& operator=(const emissionValue& o);
        const std::optional<t_emissionValue>& getAnnual() const;
        const std::optional<t_quarterEmissionValue>& getQuarter() const;
    private:
        std::optional<t_emissionValue> m_annualValue;
        std::optional<t_quarterEmissionValue> m_quarterValue;
    } t_emissionValueHandler;

    ///< nuclide info
    typedef struct nuclideInfo
    {
        double halflife;
    } t_nuclideInfo;

    ///< nuclide identificator is it's name
    typedef QString t_nuclideName;

    ///< nuclide handler
    typedef std::map<t_nuclideName, t_nuclideInfo> t_nuclidesTable;

    ///< generic id typedef
    typedef size_t t_id;

    ///< id of unique source (e.g. tube)
    typedef t_id t_sourceId;

    ///< data unit in emissions table
    typedef struct emission
    {
        t_sourceId source;
        t_nuclideName nuclideName;
        double temperature;
        t_emissionValueHandler value;
    } t_emission;

    ///< id of unique source (e.g. tube)
    typedef t_id t_emissionId;

    ///< emissions table
    typedef std::unordered_map<t_emissionId, t_emission> t_emissionsTable;

    ///< source manager with unobvious coordinates managing
    /// (but me, me knowsss yess... me won't forgetssss no...)
    class Source
    {
    public:
        typedef std::vector<emission> t_emissions;
        enum t_coordType
        {
            EPSG4326,
            EPSG3857,
            RELATIVE
        };
        t_epsg4326coord coordinates;
        double height;
        void InitCoordinates();
        void SetRawCoordinates(double x, double y);
        void SetType(t_coordType type);
    private:
        double m_rawX;
        double m_rawY;
        t_coordType m_coordType;
    };

    typedef Source t_source;

    ///< table of sources with their id's
    typedef std::unordered_map<t_sourceId, t_source> t_sourcesTable;

    ///< vector that can be treated as 2-D array via at()
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

    ///< long term meteorological information for the site of interest
    typedef std::vector<mt::t_observation> t_climateJournal;

    ///< microrelief types
    enum t_microrelief : unsigned short
    {
        MR_SNOW,
        MR_SHORTGRASS,
        MR_TALLGRASS,
        MR_SCRUB_GROWTH,
        MR_FOREST,
        MR_BUILDINGS,
        MR_WATER,
        MR_COUNT,
        MR_UNKNOWN
    };

    ///< roughness height, cm
    typedef std::array<char, MR_COUNT> t_roughness;

    ///< coefficients for f(z0, x)
    typedef struct fCoeffs
    {
        typedef std::array<double, MR_COUNT> t_vals;
        t_vals c1;
        t_vals c2;
        t_vals d1;
        t_vals d2;
    } t_fCoeffs;

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
        vectorAsArray<double> value; ///< meters
        vectorAsArray<char> mask; ///< true if corresponding point is within model
    } t_distances;

    ///< array of distances from source to point, some kind of cache
    ///< may not need it tho, if that's so, get rid of this shit
    typedef std::unordered_map<t_sourceId, t_distances> t_distancesTable;

    ///< array of concentrations for source & nuclide combination
    typedef vectorAsArray<double> t_dilutionFactors;

    ///< key here is hash of source & nuclide combination
    typedef std::unordered_map<t_emissionId, t_dilutionFactors> t_dilutionFactorsTable;

    class ConcentrationsTable;

    ///< we don't need to allocate another arrays for concentrations
    /// so it's a class storing a pointer to dilutions array and returning
    /// the concentration value by value
    class Concentrations
    {
    public:
        double at(const size_t x, const size_t y);
        double at(const size_t i);
        friend ConcentrationsTable;
    private:
        const t_dilutionFactors* m_dilutionFactors;
        t_emissionValueHandler m_emissionValue;
    };

    typedef Concentrations t_concentrations;

    ///< and this is a table which set all the t_concentrations objects
    class ConcentrationsTable :
            public std::unordered_map<t_emissionId, t_concentrations>
    {
    public:
        ConcentrationsTable(const t_dilutionFactorsTable& dilutionfactorsTable);
        void Update();
    private:
        const t_dilutionFactorsTable& m_dilutionsTable;
    };

    typedef ConcentrationsTable t_concentrationsTable;
} // namespace mt

QTextStream& operator<<(QTextStream &os, const mt::t_emission &em);

#endif // GLOBALTYPES_H
