#ifndef DATAMANAGER_H
#define DATAMANAGER_H

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
protected:
    t_status m_status;
    DataBaseManager& m_dbManager;
};

#endif // DATAMANAGER_H
