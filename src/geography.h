#ifndef GEOGRAPHY_H
#define GEOGRAPHY_H

#include "globaltypes.h"

///< максимальное расстояние, на котором работает модель рассеяния Гаусса
const double gaussian_model_limit = 30000;

double calculate_distance(mt::t_epsg4326coord p1, mt::t_epsg4326coord p2);

#endif // GEOGRAPHY_H
