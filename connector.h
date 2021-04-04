#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "mainwindow.h"
#include "datainterface.h"

class Connector : public QObject
{
    Q_OBJECT
public:
    Connector(MainWindow& window, DataInterface& data, QObject *parent = nullptr);
private slots:
    void OnClimateAccept();
    void OnClimateReset();
    void OnGeospatialAccept();
    void OnGeospatialReset();
    void OnImageAccept();
    void OnImageReset();
    void OnSourcesAccept();
    void OnSourcesReset();
private:
    MainWindow* m_window; ///< pointer to window with slots and signals
    DataInterface* m_data; ///< pointer to data interface with slots and signals
};

#endif // CONNECTOR_H
