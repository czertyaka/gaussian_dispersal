#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>
#include <QTextStream>
#include <Qstring>

#include "climaticvariables.h"
#include "geospatialdata.h"

#ifndef MY_LOG
#define MY_LOG( X ) { QString s; QTextStream str(&s); str << X; DataInterface::GetInstance().AddLog(str); }
#endif // MY_LOG

class DataInterface : public QObject
{
    Q_OBJECT
public:
    static DataInterface& GetInstance();
    void AddLog(const QTextStream& stream);

signals:
    void AddLogSignal(const QString& log);

public slots:
    void AddClimaticJournal(const QString& filename, ClimateCsvParser::t_format format);
    void AddGeospatialData(const QString& filename);

private:
    explicit DataInterface(QObject *parent = nullptr);
    ClimaticVariables* m_climaticVariables;
    GeospatialData* m_GeospatialData;

};

#endif // DATAINTERFACE_H
