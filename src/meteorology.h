#ifndef METEOROLOGY_H
#define METEOROLOGY_H

#include <QString>
#include <QDateTime>
#include <cmath>

#ifndef DATETIME_FORMAT
#define DATETIME_FORMAT "MM-dd-yyyy hh:00:00"
#endif

#if !defined(RAD) && !defined(DEG)
#define RAD(degree) static_cast<double>(degree)/180*M_PI
#define DEG(radian) static_cast<double>(radian)/M_PI*180
#endif

namespace mm
{
    enum t_windDir : const int
    {
        calm = 0,
        N    = 1,
        NNE  = 2,
        NE   = 3,
        ENE  = 4,
        E    = 5,
        ESE  = 6,
        SE   = 7,
        SSE  = 8,
        S    = 9,
        SSW  = 10,
        SW   = 11,
        WSW  = 12,
        W    = 13,
        WNW  = 14,
        NW   = 15,
        NNW  = 16
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

    struct epsg4326coord;

    typedef struct epsg3857coord
    {
        double easting;
        double northing;
        epsg3857coord(double easting = 0, double northing = 0);
        epsg3857coord(const epsg4326coord& o);
    } t_epsg3857coord;

    typedef t_epsg3857coord t_pseudoMercatorCoord;

    typedef struct epsg4326coord
    {
        double lon;
        double lat;
        epsg4326coord(double lon = 0, double lat = 0);
        epsg4326coord(const epsg3857coord& o);
    } t_epsg4326coord;

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

        matrix()
        {
            for (size_t n = 0; n < N; ++n)
            {
                for (size_t k = 0; k < K; ++k)
                {
                    windSpRepByCPCold[n][k] = 0;
                    windSpRepByCPWarm[n][k] = 0;
                    for (size_t j = 0; j < J; ++j)
                    {
                        mCold[n][j][k] = 0;
                        mWarm[n][j][k] = 0;
                        wCold[n][j][k] = 0;
                        wWarm[n][j][k] = 0;
                    }
                }
            }
        }
    } t_matrix;
}

#endif // METEOROLOGY_H
