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
    void UpdateClimateStatusLabel(const bool ok);
    void UpdateGeoStatusLabel(const bool ok);
    void UpdateImageStatusLabel(const bool ok);
    void UpdateSourcesStatusLabel(const bool ok);

public slots:
    bool AddClimaticJournal(const QString& filename, ClimateCsvParser::t_format format);
    bool AddGeospatialData(const QString& filename);

private:
    explicit DataInterface(QObject *parent = nullptr);
    ClimaticVariables* m_climaticVariables;
    GeospatialData* m_GeospatialData;

};

#endif // DATAINTERFACE_H
