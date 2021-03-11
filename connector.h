#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "mainwindow.h"
#include "datainterface.h"

class Connector
{
public:
    Connector(const MainWindow& window, const DataInterface& data);
private:
    const MainWindow* m_window; ///< pointer to window with slots and signals
    const DataInterface* m_data; ///< pointer to data interface with slots and signals
};

#endif // CONNECTOR_H
