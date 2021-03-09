#include "tabdatamanager.h"

TabDataManager::TabDataManager()
    : m_status(NOT_READY)
{

}

tabStatus_t TabDataManager::GetStatus()
{
    m_UpdateStatus();
    return m_status;
}
