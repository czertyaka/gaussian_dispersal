#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include "basecalculator.h"

class MatrixCalculator : public BaseCalculator
{
public:
    MatrixCalculator();
    virtual ~MatrixCalculator() = default;
    t_errorCode Execute() override;
};

#endif // MATRIXCALCULATOR_H
