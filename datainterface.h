#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>

#include "climaticvariables.h"

class DataInterface : public QObject
{
    Q_OBJECT
public:
    explicit DataInterface(QObject *parent = nullptr);

public slots:
    void AddClimaticJournal(const QString& filename);

private:
    ClimaticVariables* m_climaticVariables;
};

#endif // DATAINTERFACE_H
