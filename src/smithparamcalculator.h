#ifndef SMITHPARAMCALCULATOR_H
#define SMITHPARAMCALCULATOR_H

#include "basecalculator.h"
#include "globaltypes.h"

class SmithParamCalculator : public BaseCalculator
{
public:

    SmithParamCalculator(const mt::t_observation& obs);
    t_returnCode Execute() override;
    mt::t_smithParam Get() const;

private:

    bool GetCoordinate();
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

    mt::t_smithParam m_param;

    const mt::t_observation& m_obs;

    double  m_riseTime; ///< время восхода солнца, ч.
    double  m_setTime; ///< время захода солнца, ч
    double  m_sunDecl; ///< солнечное склонение, град.
    double  m_sunAngle; ///< высота солнца, град.
    int     m_insolClass; ///< класс инсоляции

    t_epsg4326coord m_coord;
};

#endif // SMITHPARAMCALCULATOR_H
