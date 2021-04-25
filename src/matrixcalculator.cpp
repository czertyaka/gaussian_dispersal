#include "matrixcalculator.h"
#include "smithparamcalculator.h"
#include "datainterface.h"

#include <QDateTime>

MatrixCalculator::MatrixCalculator() :
    BaseCalculator(),
    m_journal(m_db.ClimateJournal()),
    m_matrix(m_db.Matrix())
{

}

BaseCalculator::t_errorCode MatrixCalculator::Execute()
{  
    for (DataBase::t_climateJournal::iterator iter = m_db.ClimateJournal().begin();
         iter != m_db.ClimateJournal().end(); ++iter)
    {
        if (!AddObservation(*iter))
        {
            MY_LOG("error while adding observation " << iter->dateTime.toString() << " to matrix");
            return ERROR;
        }
    }

    NormalizeMatrix();
//    CalcWindRose();
//    CalcWindSpeedRepeatabilityByCompPoint();
//    CalcWindSpeedRepeatability();
//    CalcCalmRepeatability();
//    CalcSmithParamRepeatability();
//    CalcAverageWindSpeedByCompPoint();
//    CalcAverageWindSpeedBySmithParam();

    return OK;
}

bool MatrixCalculator::AddObservation(const mm::t_observation &obs)
{    
    SmithParamCalculator smithParamCalculator(obs);
    if (smithParamCalculator.Execute() == ERROR)
    {
        MY_LOG("error while calculating smith parameter");
        return false;
    }

    bool result = true;;

    int j = GetJ(smithParamCalculator.Get(), result);
    int k = GetK(obs.windSpeed, result);
    int n = GetN(obs.windDir, result);

    if (!result)
    {
        return false;
    }

    if ((obs.dateTime.date().month() >= mm::APR && obs.dateTime.date().day() > 15)
            || (obs.dateTime.date().month() <= mm::OCT && obs.dateTime.date().day() <= 15))
    {
        m_matrix.mWarm[n][j][k]++;
        m_matrix.MWarm++;
        if (k) { m_matrix.MWarmNoCalm++; }
    }
    else
    {
        m_matrix.mCold[n][j][k]++;
        m_matrix.MCold++;
        if (k) { m_matrix.MColdNoCalm++; }
    }

    return true;
}

size_t MatrixCalculator::GetJ(const mm::t_smithParam smithParam, bool& result) const
{
    switch (smithParam)
    {
    case mm::cathA: return 0;
    case mm::cathB: return 1;
    case mm::cathC: return 2;
    case mm::cathD: return 3;
    case mm::cathE: return 4;
    case mm::cathF: return 5;
    case mm::cathG: return 6;
    default:
        MY_LOG("invalid smith param " << (int)smithParam);
        result = false;
        break;
    }

    return 0;
}

size_t MatrixCalculator::GetK(const double windSpeed, bool &result) const
{
    if (windSpeed < 0)
    {
        MY_LOG("negative wind speed " << windSpeed << " м/с.");
        result = false;
    }
    else if (windSpeed < 0.75) { return 0; }
    else if (windSpeed < 1.5)  { return 1; }
    else if (windSpeed < 2.5)  { return 2; }
    else if (windSpeed < 3.5)  { return 3; }
    else if (windSpeed < 5.5)  { return 4; }
    else if (windSpeed < 7.5)  { return 5; }
    else if (windSpeed < 10)   { return 6; }
    else                       { return 7; }

    return 0;
}

size_t MatrixCalculator::GetN(const mm::t_windDir windDir, bool &result) const
{
    switch (windDir)
    {
    case mm::calm: return 0;
    case mm::N:    return 0;
    case mm::NNE:  return 1;
    case mm::NE:   return 2;
    case mm::ENE:  return 3;
    case mm::E:    return 4;
    case mm::ESE:  return 5;
    case mm::SE:   return 6;
    case mm::SSE:  return 7;
    case mm::S:    return 8;
    case mm::SSW:  return 9;
    case mm::SW:   return 10;
    case mm::WSW:  return 11;
    case mm::W:    return 12;
    case mm::WNW:  return 13;
    case mm::NW:   return 14;
    case mm::NNW:  return 15;
    default:
        MY_LOG("invalid wind direction " << (int)windDir);
        result = false;
    }

    return 0;
}

void MatrixCalculator::NormalizeMatrix()
{
    // нормирование для элементов с k > 0
    for (std::size_t n = 0; n < m_matrix.N; n++) {
        for (std::size_t j = 0; j < m_matrix.J; j++) {
            for (std::size_t k = 1; k < m_matrix.K; k++) {
                m_matrix.wCold[n][j][k] = m_matrix.MCold != 0 ?
                                    static_cast<double>(m_matrix.mCold[n][j][k]) /
                                    static_cast<double>(m_matrix.MCold) : 0;
                m_matrix.wWarm[n][j][k] = m_matrix.MWarm != 0 ?
                                    static_cast<double>(m_matrix.mWarm[n][j][k]) /
                                    static_cast<double>(m_matrix.MWarm) : 0;
            }
        }
    }

    // нормирование для элементов с k = 0
    int sum2Cold = 0;
    int sum2Warm = 0;
    for (std::size_t n = 0; n < m_matrix.N; n++) {
        for (std::size_t j = 0; j < m_matrix.J; j++) {
            sum2Cold += m_matrix.mCold[n][j][1];
            sum2Warm += m_matrix.mWarm[n][j][1];
        }
    }

    for (std::size_t n = 0; n < m_matrix.N; n++) {

        int sum1Cold = 0;
        int sum1Warm = 0;
        for (std::size_t j = 0; j < m_matrix.J; j++) {
            sum1Cold += m_matrix.mCold[n][j][1];
            sum1Warm += m_matrix.mWarm[n][j][1];
        }

        for (std::size_t j = 0; j < m_matrix.J; j++) {
            m_matrix.wCold[n][j][0] = m_matrix.MCold != 0 ?
                                static_cast<double>(m_matrix.mCold[0][j][0]) /
                                static_cast<double>(m_matrix.MCold) *
                                static_cast<double>(sum1Cold) /
                                static_cast<double>(sum2Cold) :
                                0;
            m_matrix.wWarm[n][j][0] = m_matrix.MWarm != 0 ?
                                static_cast<double>(m_matrix.mWarm[0][j][0]) /
                                static_cast<double>(m_matrix.MWarm) *
                                static_cast<double>(sum1Warm) /
                                static_cast<double>(sum2Warm) :
                                0;
        }
    }

    // проверка нормированности
    double wColdSum = 0;
    double wWarmSum = 0;
    for (std::size_t n = 0; n < m_matrix.N; n++) {
        for (std::size_t j = 0; j < m_matrix.J; j++) {
            for (std::size_t k = 0; k < m_matrix.K; k++) {
                wColdSum += m_matrix.wCold[n][j][k];
                wWarmSum += m_matrix.wWarm[n][j][k];
            }
        }
    }

    if (abs(wColdSum - 1) > 0.0001 && m_matrix.MCold != 0) {
        MY_LOG("проверка нормированности холодной матрицы не сходится. Сумма элементов: "
             << wColdSum);
    }

    if (abs(wWarmSum - 1) > 0.0001 && m_matrix.MWarm != 0) {
        MY_LOG("Проверка нормированности теплой матрицы не сходится. Сумма элементов: "
             << wWarmSum);
    }
}
