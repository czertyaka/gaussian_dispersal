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

    bool AddObservation(const mt::t_observation& obs);

    size_t GetJ(const mt::t_smithParam smithParam, bool& result) const;
    size_t GetK(const double windSpeed, bool& result) const;
    size_t GetN(const mt::t_windDir, bool& result) const;

    void NormalizeMatrix();
    void CalcWindRose();
    void CalcWindSpeedRepeatabilityByCompPoint();
    void CalcWindSpeedRepeatability();
    void CalcCalmRepeatability();
    void CalcSmithParamRepeatability();
    void CalcAverageWindSpeedByCompPoint();
    void CalcAverageWindSpeedBySmithParam();

    DataBase::t_climateJournal& m_journal;
    mt::t_matrix&               m_matrix;
};

#endif // MATRIXCALCULATOR_H
