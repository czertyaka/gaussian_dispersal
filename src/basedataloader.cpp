#include "basedataloader.h"
#include "database.h"
#include "datainterface.h"

BaseDataLoader::BaseDataLoader()
    : m_status(NOT_READY)
    , m_db(DataBase::GetInstance())
{

}
