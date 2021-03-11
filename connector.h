#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "mainwindow.h"
#include "datainterface.h"

class Connector
{
public:
    Connector(const MainWindow& window, const DataInterface& data);
private:
    const MainWindow* m_window;
    const DataInterface* m_data;
};

#endif // CONNECTOR_H
