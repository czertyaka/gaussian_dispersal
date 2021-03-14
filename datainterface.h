#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>

#include "climaticvariables.h"
#include "climatecsvparser.h"

class DataInterface : public QObject
{
    Q_OBJECT
public:
    explicit DataInterface(QObject *parent = nullptr);

public slots:
    void AddClimaticJournal(const QString& filename, ClimateCsvParser::t_format format);

private:
    ClimaticVariables* m_climaticVariables;
};

#endif // DATAINTERFACE_H
