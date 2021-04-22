#include "database.h"

DataBase &DataBase::GetInstance()
{
    static DataBase Instance;
    return Instance;
}

DataBase::~DataBase()
{
    delete m_climateJournal;
    delete m_landscape;
    delete m_image;
    delete m_sources;
}

DataBase::t_climateJournal* DataBase::GetClimateJournal()
{
    return m_climateJournal;
}

DataBase::t_landscape* DataBase::GetLandscape()
{
    return m_landscape;
}

DataBase::t_image* DataBase::GetImage()
{
    return m_image;
}

DataBase::t_sources* DataBase::GetSources()
{
    return m_sources;
}

DataBase::DataBase() :
    m_climateJournal(new t_climateJournal),
    m_landscape(new t_landscape),
    m_image(new t_image),
    m_sources(new t_sources)
{
}
