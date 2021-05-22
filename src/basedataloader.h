#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>

class DataBase;

class BaseDataLoader
{
public:
    enum t_status
    {
        NOT_READY,
        READY,
        ERROR
    };
    BaseDataLoader();
    t_status CheckStatus() const { return m_status; }
    virtual void Reset() = 0;
protected:
    t_status m_status;
    DataBase& m_db;
};

#endif // DATAMANAGER_H
