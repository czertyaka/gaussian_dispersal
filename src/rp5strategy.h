#ifndef RP5CSVPARSER_H
#define RP5CSVPARSER_H

#include "globaltypes.h"
#include "climatecsvparser.h"
#include "climaticparsingstrategy.h"

class Rp5Strategy : public ClimaticParsingStrategy
{
public:
    Rp5Strategy();
    CsvParser::t_lineStatus ParseLine(const QStringList& list, mt::t_observation& observation) override;
private:
    bool ParseTime();
    void ParseTemper();
    bool ParseWindDir();
    void ParseWindSpeed();
    bool ParseCloudCoverage();
    void ParseFog();
    void ParseSnow();
    QString m_Time;
    QString m_T;
    QString m_DD;
    QString m_Ff;
    QString m_N;
    QString m_Nh;
    QString m_VV;
    QString m_E1;
    QString m_prevE1;
    QString m_E2;
    QString m_prevE2;
};

#endif // RP5CSVPARSER_H
