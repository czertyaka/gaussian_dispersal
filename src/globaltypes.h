#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

#include <QString>
#include <QDateTime>
#include <cmath>

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
    enum t_windDir : const int
    {
        N,
        NNE,
        NE,
        ENE,
        E,
        ESE,
        SE,
        SSE,
        S,
        SSW,
        SW,
        WSW,
        W,
        WNW,
        NW,
        NNW,
        WIND_DIR_COUNT = NNW,
        calm
    };

    enum t_smithParam : const int
    {
        cathA = 1,
        cathB = 2,
        cathC = 3,
        cathD = 4,
        cathE = 5,
        cathF = 6,
        cathG = 7
    };

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

    typedef struct matrix
    {
        static const int N = 16; ///< количество интервалов направления ветра
        static const int J = 7; ///< количество интервалов категорий устойчивости атмосферы
        static const int K = 8; ///< количество интервалов скорости ветра

        const double windDirVals[N] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225,
                                       247.5, 270, 292.5, 315, 337.5};
        const char smithParamVals[J] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
        const double windSpeedVals[K] = { 0.5, 1, 2, 3, 4.5, 6.5, 9, 12};

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

        matrix();
    } t_matrix;

    typedef struct quarterEmission
    {
        double fisrtQuarter;
        double secondQuarter;
        double thirdQuarter;
        double fourthQuarter;
    } t_quarterEmission;

    typedef class emissionValue
    {
    public:
        emissionValue();
        emissionValue(double annualValue);
        emissionValue(double fisrt, double second, double third, double fourth);
        emissionValue& operator=(const emissionValue& o);
        const std::optional<double>& getAnnual() const;
        const std::optional<t_quarterEmission>& getQuarter() const;
    private:
        std::optional<double> m_annualValue;
        std::optional<t_quarterEmission> m_quarterValue;
    } t_emissionValue;

    typedef struct nuclide
    {
        QString name;
        double halfLife;
        bool operator<(const nuclide& o) const;
        bool operator==(const nuclide& o) const;
    } t_nuclide;

    typedef struct emission
    {
        QString nuclideName;
        double temperature;
        t_emissionValue value;
    } t_emission;

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
        t_emissions emissions;
        void InitCoordinates();
        void SetRawCoordinates(double x, double y);
        void SetType(t_coordType type);
    private:
        double m_rawX;
        double m_rawY;
        t_coordType m_coordType;
    };

    typedef Source t_source;
}

#endif // GLOBALTYPES_H
