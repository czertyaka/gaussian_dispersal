#include "connector.h"

/**
 * @brief Connector::Connector
 * @param window
 * @param data
 */
Connector::Connector(const MainWindow& window, const DataInterface& data)
    : m_window(&window)
    , m_data(&data)
{

}
