#ifndef DILUTIONSCALCULATOR_H
#define DILUTIONSCALCULATOR_H

#include "basecalculator.h"

class DilutionsCalculator : public BaseCalculator
{
public:
    DilutionsCalculator();
     t_returnCode Execute() override;
};

#endif // DILUTIONSCALCULATOR_H
