#ifndef TabManager_H
#define TabManager_H

#include <QApplication>

enum tabStatus_t
{
    NOT_READY,
    READY,
    ERROR
};

class TabManager : public QObject
{
    Q_OBJECT
public:
    TabManager();
    tabStatus_t GetStatus() { return m_status; }
signals:
    void StatusChanged();
protected:
    tabStatus_t m_status;
};

#endif // TabManager_H
