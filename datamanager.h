#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>

class DataBaseManager;

class DataManager
{
public:
    enum t_status
    {
        NOT_READY,
        READY,
        ERROR
    };
    DataManager();
    t_status CheckStatus() const { return m_status; }
    virtual void Reset() = 0;
protected:
    t_status m_status;
    DataBaseManager& m_dbManager;
    bool CheckPointer(void* ptr, const QString& msg);
};

#endif // DATAMANAGER_H
