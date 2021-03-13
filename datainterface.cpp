#include "datainterface.h"

/**
 * @brief DataInterface::DataInterface
 * @param parent
 */
DataInterface::DataInterface(QObject *parent)
    : QObject(parent)
    , m_climaticVariables(new ClimaticVariables())
{

}

void DataInterface::AddClimaticJournal(const QString &filename)
{
    m_climaticVariables->AddJournal(filename);
}
