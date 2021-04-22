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
    bool CheckPointer(void* ptr, const QString& msg);
};

#endif // DATAMANAGER_H
