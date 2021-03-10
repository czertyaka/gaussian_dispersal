#ifndef CLIMATETABMANAGER_H
#define CLIMATETABMANAGER_H

#include <QString>

#include "tabmanager.h"

enum ClimatePresets_t
{
    NONE    = 0,
    ANOTHER = 1,
    URAL    = 2,
    SYBERIA = 3
};

class ClimateTabManager : public TabManager
{
public:
    ClimateTabManager();
    void Init(const QString& filename);
    void Init(const ClimatePresets_t preset);
};

#endif // CLIMATETABMANAGER_H
