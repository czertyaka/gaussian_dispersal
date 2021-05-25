#ifndef BASECALCULATOR_H
#define BASECALCULATOR_H

class DataBase;

class BaseCalculator
{
public:
    BaseCalculator();
    enum t_returnCode
    {
        OK,
        ERROR
    };
    virtual t_returnCode Execute() = 0;
protected:
    DataBase& m_db;
    mutable t_returnCode m_error;
};

#endif // BASECALCULATOR_H
