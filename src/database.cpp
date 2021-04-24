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
    delete m_matrix;
}

mm::t_matrix &DataBase::Matrix()
{
    return *m_matrix;
}

DataBase::t_climateJournal& DataBase::ClimateJournal()
{
    return *m_climateJournal;
}

DataBase::t_landscape& DataBase::Landscape()
{
    return *m_landscape;
}

DataBase::t_image& DataBase::Image()
{
    return *m_image;
}

DataBase::t_sources& DataBase::Sources()
{
    return *m_sources;
}

DataBase::DataBase() :
    m_climateJournal(new t_climateJournal),
    m_landscape(new t_landscape),
    m_image(new t_image),
    m_sources(new t_sources),
    m_matrix(new mm::t_matrix)
{
}
