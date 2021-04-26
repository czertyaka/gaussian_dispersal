#include "database.h"
#include "csvwriter.h"
#include "datainterface.h"

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

bool DataBase::SaveMatrix(const QString& directory)
{
    {
        QString filename = directory + "/wind-rose.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Wind rose");
        writer.AddItem("Compass point, deg.");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t n = 0; n < m_matrix->N; ++n)
        {
            writer.AddItem(m_matrix->windDirVals[n]);
            writer.AddItem(m_matrix->windRoseCold[n]);
            writer.AddItem(m_matrix->windRoseWarm[n]);
        }

        MY_LOG("finished writing to " << filename);
    }

    return true;
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
