#ifndef DataInterface_H
#define DataInterface_H

#include <QObject>
#include <QTextStream>
#include <QString>

#include "climaticvariablesloader.h"
#include "geospatialdataloader.h"
#include "sourcesloader.h"
#include "imageloader.h"
#include "matrixcalculator.h"

#ifndef MY_LOG
#define MY_LOG( X ) { QString s; QTextStream str(&s); str << (__PRETTY_FUNCTION__) << ": " << X; DataInterface::GetInstance().AddLog(str); }
#endif // MY_LOG

class DataInterface : public QObject
{
    Q_OBJECT
public:
    static DataInterface& GetInstance();
    ~DataInterface();
    void AddLog(const QTextStream& stream);
    bool CheckStatus();

signals:
    void AddLogSignal(const QString& log);
    void UpdateClimateStatusLabel(const bool ok);
    void UpdateGeoStatusLabel(const bool ok);
    void UpdateImageStatusLabel(const bool ok);
    void UpdateSourcesStatusLabel(const bool ok);

public slots:
    bool AddClimaticJournal(const QString& filename, ClimateCsvParser::t_format format);
    void OnClimateReset();
    bool AddGeospatialData(const QString& filename);
    void OnGeospatialReset();
    bool AddImage(const QString& filename, const ImageLoader::t_optBorders& optBorders);
    void OnImageReset();
    bool AddSources(const SourcesLoader::t_vSources& vSources);
    void OnSourcesReset();
    void OnStart();

private:
    explicit DataInterface(QObject *parent = nullptr);
    ClimaticVariablesLoader*    m_climaticVariablesLoader;
    GeospatialDataLoader*       m_geospatialDataLoader;
    ImageLoader*                m_imageLoader;
    SourcesLoader*              m_sourcesLoader;
    MatrixCalculator*           m_matrixCalculator;

};

#endif // DataInterface_H
