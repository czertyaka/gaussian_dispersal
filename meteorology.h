#ifndef METEOROLOGY_H
#define METEOROLOGY_H

#include <QString>

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

    enum t_month : const int
    {
        january     = 1,
        february    = 2,
        march       = 3,
        april       = 4,
        may         = 5,
        june        = 6,
        july        = 7,
        august      = 8,
        september   = 9,
        october     = 10,
        november    = 11,
        december    = 12
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

    typedef struct observation
    {
        int day; ///< день
        t_month month; ///< месяц
        int year; ///< год
        int time; ///< гринвичевское время

        double latitude; ///< широта, град.
        double longitude; ///< долгота, град.

        t_windDir windDir; ///< направление ветра, румб
        t_smithParam smithParam; ///< параметр Смита (категория устойчивости атмосферы)
        double windSpeed; ///< скорость ветра, м/с

        int cloudAmount; ///< балл общей облачности, от 0 до 10
        int lowerCloudAmount; ///< балл нижней облачности, от 0 до 10
        bool fog; ///< наличие тумана
        bool snow; ///< наличие сплошного снежного покрова

        double temper; ///< температура, град. Цельсия
    } t_observation;
}

#endif // METEOROLOGY_H
