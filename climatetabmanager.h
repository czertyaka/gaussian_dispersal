#ifndef CLIMATETABMANAGER_H
#define CLIMATETABMANAGER_H

#include <QString>

#include "tabdatamanager.h"

enum ClimatePresets_t
{
    NONE    = 0,
    ANOTHER = 1,
    URAL    = 2,
    SYBERIA = 3
};

class ClimateTabManager : public TabDataManager
{
public:
    ClimateTabManager();
    virtual ~ClimateTabManager() {}
    void Init(const QString& filename);
    void Init(const ClimatePresets_t preset);
private:
    void m_UpdateStatus() final;
};

#endif // CLIMATETABMANAGER_H
