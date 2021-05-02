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

mt::t_matrix &DataBase::Matrix()
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

    {
        QString filename = directory + "/wind-speed-repetitions.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Absolute wind speed value repetitions");
        writer.AddItem("Wind speed, m/s");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t k = 0; k < m_matrix->K; ++k)
        {
            writer.AddItem(m_matrix->windSpeedVals[k]);
            writer.AddItem(m_matrix->windSpRepCold[k]);
            writer.AddItem(m_matrix->windSpRepWarm[k]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/calm-repetitions.csv";
        CsvWriter writer(filename, 2);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Calm repetitions");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");
        writer.AddItem(m_matrix->calmRepCold);
        writer.AddItem(m_matrix->calmRepWarm);

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/smith-param-repetitions.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Smith parameter values repetitions");
        writer.AddItem("Smith parameter");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t j = 0; j < m_matrix->J; ++j)
        {
            writer.AddItem(m_matrix->smithParamVals[j]);
            writer.AddItem(m_matrix->smithParamRepCold[j]);
            writer.AddItem(m_matrix->smithParamRepWarm[j]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/wind-speed-by-direction.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Wind speed values by direction, m/s");
        writer.AddItem("Compass point, deg.");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t n = 0; n < m_matrix->N; ++n)
        {
            writer.AddItem(m_matrix->windDirVals[n]);
            writer.AddItem(m_matrix->avWindSpByCPCold[n]);
            writer.AddItem(m_matrix->avWindSpByCPWarm[n]);
        }

        MY_LOG("finished writing to " << filename);
    }

    {
        QString filename = directory + "/wind-speed-by-smith-param.csv";
        CsvWriter writer(filename, 3);
        if (!writer.Init())
        {
            MY_LOG("error writing to " << filename);
            return false;
        }

        writer.AddComment("Wind speed values by direction, m/s");
        writer.AddItem("Smith param");
        writer.AddItem("Cold season");
        writer.AddItem("Warm season");

        for (size_t j = 0; j < m_matrix->J; ++j)
        {
            writer.AddItem(m_matrix->smithParamVals[j]);
            writer.AddItem(m_matrix->avWindSpBySPCold[j]);
            writer.AddItem(m_matrix->avWindSpBySPWarm[j]);
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
    m_matrix(new mt::t_matrix)
{
}
