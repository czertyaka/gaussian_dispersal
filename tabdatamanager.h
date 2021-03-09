#ifndef TABDATAMANAGER_H
#define TABDATAMANAGER_H

enum tabStatus_t
{
    NOT_READY,
    READY,
    ERROR
};

class TabDataManager
{
public:
    TabDataManager();
    tabStatus_t GetStatus(); ///< get current status of tab
protected:
    tabStatus_t m_status;
private:
    virtual void m_UpdateStatus() = 0;
};

#endif // TABDATAMANAGER_H
