#ifndef BASECALCULATOR_H
#define BASECALCULATOR_H

class DataBase;

class BaseCalculator
{
public:
    BaseCalculator();
    enum t_errorCode
    {
        OK,
        NOT_SUPPORTED
    };
    virtual t_errorCode Execute() = 0;
protected:
    DataBase& m_db;
};

#endif // BASECALCULATOR_H
