#include "climatetabmanager.h"

ClimateTabManager::ClimateTabManager()
    : TabManager()
{

}

void ClimateTabManager::Init(const QString& filename)
{
    m_status = READY; //TODO
    emit StatusChanged();
}

void ClimateTabManager::Init(const ClimatePresets_t preset)
{
    m_status = READY; //TODO
    emit StatusChanged();
}
