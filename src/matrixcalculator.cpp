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
    CalcWindRose();
    CalcWindSpeedRepeatabilityByCompPoint();
    CalcWindSpeedRepeatability();
    CalcCalmRepeatability();
    CalcSmithParamRepeatability();
    CalcAverageWindSpeedByCompPoint();
    CalcAverageWindSpeedBySmithParam();

    return OK;
}

bool MatrixCalculator::AddObservation(const mt::t_observation &obs)
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

    if ((obs.dateTime.date().month() >= mt::APR && obs.dateTime.date().day() > 15)
            || (obs.dateTime.date().month() <= mt::OCT && obs.dateTime.date().day() <= 15))
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

size_t MatrixCalculator::GetJ(const mt::t_smithParam smithParam, bool& result) const
{
    switch (smithParam)
    {
    case mt::cathA: return 0;
    case mt::cathB: return 1;
    case mt::cathC: return 2;
    case mt::cathD: return 3;
    case mt::cathE: return 4;
    case mt::cathF: return 5;
    case mt::cathG: return 6;
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

size_t MatrixCalculator::GetN(const mt::t_windDir windDir, bool &result) const
{
    switch (windDir)
    {
    case mt::calm: return 0;
    case mt::N:    return 0;
    case mt::NNE:  return 1;
    case mt::NE:   return 2;
    case mt::ENE:  return 3;
    case mt::E:    return 4;
    case mt::ESE:  return 5;
    case mt::SE:   return 6;
    case mt::SSE:  return 7;
    case mt::S:    return 8;
    case mt::SSW:  return 9;
    case mt::SW:   return 10;
    case mt::WSW:  return 11;
    case mt::W:    return 12;
    case mt::WNW:  return 13;
    case mt::NW:   return 14;
    case mt::NNW:  return 15;
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

void MatrixCalculator::CalcWindRose()
{
    for (size_t n = 0; n < m_matrix.N; n++) {

        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t j = 0; j < m_matrix.J; j++) {
            for (size_t k = 1; k < m_matrix.K; k++) {
                sumCold += m_matrix.mCold[n][j][k];
                sumWarm += m_matrix.mWarm[n][j][k];
            }
        }

        // calc rose
        m_matrix.windRoseCold[n] = m_matrix.MColdNoCalm != 0 ?
                             static_cast<double>(sumCold) /
                             static_cast<double>(m_matrix.MColdNoCalm) : 0;
        m_matrix.windRoseWarm[n] = m_matrix.MWarmNoCalm != 0 ?
                             static_cast<double>(sumWarm) /
                             static_cast<double>(m_matrix.MWarmNoCalm) : 0;
    }
}

void MatrixCalculator::CalcWindSpeedRepeatabilityByCompPoint()
{
    for (size_t n = 0; n < m_matrix.N; n++) {
        for (size_t k = 0; k < m_matrix.K; k++) {

            // calc sums
            int sumCold = 0;
            int sumWarm = 0;
            for (size_t j = 0; j < m_matrix.J; j++) {
                sumCold += m_matrix.mCold[n][j][k];
                sumWarm += m_matrix.mWarm[n][j][k];
            }

            // calc reps
            m_matrix.windSpRepByCPCold[n][k] = m_matrix.MCold != 0 ?
                                         static_cast<double>(sumCold) /
                                         static_cast<double>(m_matrix.MCold) : 0;
            m_matrix.windSpRepByCPWarm[n][k] = m_matrix.MWarm != 0 ?
                                         static_cast<double>(sumWarm) /
                                         static_cast<double>(m_matrix.MWarm) : 0;
        }
    }
}

void MatrixCalculator::CalcWindSpeedRepeatability()
{
    for (size_t k = 0; k < m_matrix.K; k++) {

        m_matrix.windSpRepCold[k] = 0;
        m_matrix.windSpRepWarm[k] = 0;

        for (size_t n = 0; n < m_matrix.N; n++) {

            m_matrix.windSpRepCold[k] += m_matrix.windSpRepByCPCold[n][k];
            m_matrix.windSpRepWarm[k] += m_matrix.windSpRepByCPWarm[n][k];
        }
    }
}

void MatrixCalculator::CalcCalmRepeatability()
{
    m_matrix.calmRepCold = m_matrix.windSpRepByCPCold[0][0];
    m_matrix.calmRepWarm = m_matrix.windSpRepByCPWarm[0][0];
}

void MatrixCalculator::CalcSmithParamRepeatability()
{
    for (size_t j = 0; j < m_matrix.J; j++) {

        // cals sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t n = 0; n < m_matrix.N; n++) {
            for (size_t k = 0; k < m_matrix.K; k++) {
                sumCold += m_matrix.mCold[n][j][k];
                sumWarm += m_matrix.mWarm[n][j][k];
            }
        }

        // calc reps
        m_matrix.smithParamRepCold[j] = m_matrix.MCold != 0 ?
                                  static_cast<double>(sumCold) /
                                  static_cast<double>(m_matrix.MCold) : 0;
        m_matrix.smithParamRepWarm[j] = m_matrix.MWarm != 0 ?
                                  static_cast<double>(sumWarm) /
                                  static_cast<double>(m_matrix.MWarm) : 0;
    }
}

void MatrixCalculator::CalcAverageWindSpeedByCompPoint()
{
    for (size_t n = 0; n < m_matrix.N; n++) {

        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t k = 0; k < m_matrix.K; k++) {
            for (size_t j = 0; j < m_matrix.J; j++) {
                sumCold += m_matrix.mCold[n][j][k] * m_matrix.windSpeedVals[k];
                sumWarm += m_matrix.mWarm[n][j][k] * m_matrix.windSpeedVals[k];
            }
        }

        // calc speeds
        m_matrix.avWindSpByCPCold[n] = m_matrix.MCold != 0 ?
                                 static_cast<double>(sumCold) / static_cast<double>(m_matrix.MCold) :
                                 0;
        m_matrix.avWindSpByCPWarm[n] = m_matrix.MCold != 0 ?
                                 static_cast<double>(sumWarm) / static_cast<double>(m_matrix.MWarm) :
                                 0;
    }
}

void MatrixCalculator::CalcAverageWindSpeedBySmithParam()
{
    for (size_t j = 0; j < m_matrix.J; j++) {

        // calc sums
        int sumCold = 0;
        int sumWarm = 0;
        for (size_t k = 0; k < m_matrix.K; k++) {
            for (size_t n = 0; n < m_matrix.N; n++) {
                sumCold += m_matrix.mCold[n][j][k] * m_matrix.windSpeedVals[k];
                sumWarm += m_matrix.mWarm[n][j][k] * m_matrix.windSpeedVals[k];
            }
        }

        // calc speeds
        m_matrix.avWindSpBySPCold[j] = m_matrix.MCold != 0 ?
                                 static_cast<double>(sumCold) / static_cast<double>(m_matrix.MCold) :
                                 0;
        m_matrix.avWindSpBySPWarm[j] = m_matrix.MWarm != 0 ?
                                 static_cast<double>(sumWarm) / static_cast<double>(m_matrix.MWarm) :
                                 0;
    }
}
