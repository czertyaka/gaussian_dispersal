#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include "basecalculator.h"
#include "database.h"

class MatrixCalculator : public BaseCalculator
{
public:
    MatrixCalculator();
    virtual ~MatrixCalculator() = default;
    t_errorCode Execute() override;
private:

    bool AddObservation(const mm::t_observation& obs);

    size_t GetJ(const mm::t_smithParam smithParam) const;
    size_t GetK(const double windSpeed) const;
    size_t GetN(const mm::t_windDir) const;

    DataBase::t_climateJournal& m_journal;
    mm::t_matrix&               m_matrix;
};

#endif // MATRIXCALCULATOR_H
