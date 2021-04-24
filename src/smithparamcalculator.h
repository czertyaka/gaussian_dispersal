#ifndef SMITHPARAMCALCULATOR_H
#define SMITHPARAMCALCULATOR_H

#include "basecalculator.h"
#include "meteorology.h"

class SmithParamCalculator : public BaseCalculator
{
public:

    SmithParamCalculator(const mm::t_observation& obs);
    t_errorCode Execute() override;
    mm::t_smithParam Get() const;

private:

    void CalcSunAngle();
    void CalcSetOfDay();
    void CalcSunDeclination();
    void CalcInsolClass();
    double CalcAlbedo();
    void CloudCorrection();
    void FogCorrection();
    void SnowCorrection();
    void CorrectionI();
    void CorrectionII();
    void CorrectionIII();
    void CorrectionIV();
    void CorrectionV();
    void CorrectionVI();
    void CorrectionVII();

    mm::t_smithParam m_param;

    const mm::t_observation& m_obs;

    double  m_riseTime; ///< время восхода солнца, ч.
    double  m_setTime; ///< время захода солнца, ч
    double  m_sunDecl; ///< солнечное склонение, град.
    double  m_sunAngle; ///< высота солнца, град.
    int     m_insolClass; ///< класс инсоляции

    double m_latitude;
    double m_longitude;
};

#endif // SMITHPARAMCALCULATOR_H
