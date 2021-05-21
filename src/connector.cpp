#include "connector.h"
#include "ui_mainwindow.h"
#include "climatecsvparser.h"
#include "datainterface.h"
#include "sourcesloader.h"
#include "imageloader.h"

#include <QPushButton>
#include <QGridLayout>
#include <QObject>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <algorithm>
#include <functional>

#ifndef UI
    #define UI m_window->Ui()
#else
    #error
#endif

/**
 * @brief Connector::Connector
 * @param window
 * @param data
 */
Connector::Connector(MainWindow& window, DataInterface& data, QObject* parent)
    : QObject(parent)
    , m_window(&window)
    , m_data(&data)
{
    // log
    connect(m_data, &DataInterface::AddLogSignal, UI->logPlainText, &QPlainTextEdit::appendPlainText);

    // climatic variables
    connect(UI->climateAcceptButton, &QPushButton::clicked, this, &Connector::OnClimateAccept);
    connect(UI->climateResetButton, &QPushButton::clicked, this, &Connector::OnClimateReset);
    connect(UI->climateAcceptButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(UI->climateResetButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(m_data, &DataInterface::UpdateClimateStatusLabel, m_window, &MainWindow::UpdateClimateStatusLabel);
    connect(m_data, &DataInterface::MatrixDone, m_window, &MainWindow::OnClimateDone);
    connect(UI->climateSaveButton, &QPushButton::clicked, m_data, [=](){ m_data->SaveClimate(m_window->CurrentDirectory()); } );

    // geospatial data
    connect(UI->geoAcceptButton, &QPushButton::clicked, this, &Connector::OnGeospatialAccept);
    connect(UI->geoResetButton, &QPushButton::clicked, this, &Connector::OnGeospatialReset);
    connect(UI->geoAcceptButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(UI->geoResetButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(m_data, &DataInterface::UpdateGeoStatusLabel, m_window, &MainWindow::UpdateGeoStatusLabel);
    connect(m_data, &DataInterface::TerrainDone, m_window, &MainWindow::OnTerrainDone);
    connect(UI->geoSaveButton, &QPushButton::clicked, m_data, [=](){ m_data->SaveTerrain(m_window->CurrentDirectory()); } );

    // background image
    connect(UI->imageAcceptButton, &QPushButton::clicked, this, &Connector::OnImageAccept);
    connect(UI->imageResetButton, &QPushButton::clicked, this, &Connector::OnImageReset);
    connect(UI->imageAcceptButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(UI->imageResetButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(m_data, &DataInterface::UpdateImageStatusLabel, m_window, &MainWindow::UpdateImageStatusLabel);

    // sources
    connect(UI->srcAcceptButton, &QPushButton::clicked, this, &Connector::OnSourcesAccept);
    connect(UI->srcResetButton, &QPushButton::clicked, this, &Connector::OnSourcesReset);
    connect(UI->srcAcceptButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(UI->srcResetButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(m_data, &DataInterface::UpdateSourcesStatusLabel, m_window, &MainWindow::UpdateSourcesStatusLabel);

    // calculations
    connect(UI->startButton, &QPushButton::clicked, m_data, &DataInterface::OnStart);
}

void Connector::OnClimateAccept()
{
    if (UI->climateLoadRadioButton->isChecked())
    {
        QString filename = UI->climateLineEdit->text();
        ClimateCsvParser::t_format format = ClimateCsvParser::RP5;
        m_data->AddClimaticJournal(filename, format);
    }
    else
    {
        //TODO
    }
}

void Connector::OnClimateReset()
{
    m_data->OnClimateReset();
}

void Connector::OnGeospatialAccept()
{
    if (UI->geoLoadRadioButton->isChecked())
    {
        QString filename = UI->geoLineEdit->text();
        m_data->AddGeospatialData(filename);
    }
    else
    {
        //TODO
    }
}

void Connector::OnGeospatialReset()
{
    m_data->OnGeospatialReset();
}

void Connector::OnImageAccept()
{
    QString filename;
    dbt::t_optBorders optBorders;

    if (UI->imageLoadRadioButton->isChecked())
    {
        filename = UI->imageLineEdit->text();
    }
    else
    {
        // TODO
    }

    if (UI->imageCustomizeRadioButton)
    {
        // TODO
    }

    m_data->AddImage(filename, optBorders);
}

void Connector::OnImageReset()
{
    m_data->OnImageReset();
}

void Connector::OnSourcesAccept()
{
    dbt::t_sources sources;
    QGridLayout* table = UI->sourceTableLayout;

    for (int row = 1; row < m_window->SourcesRows(); ++row)
    {
        // получим координаты источника
        double x = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 2)->widget())->value();
        double y = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 3)->widget())->value();

        // создадим объект источника с такими координатами
        mt::t_source source;
        source.SetRawCoordinates(x, y);

        if (UI->coordinatesEPSG4326RadioButton->isChecked())
        {
            source.SetType(mt::t_source::EPSG4326);
        }
        else if (UI->coordinatesEPSG3857RadioButton->isChecked())
        {
            source.SetType(mt::t_source::EPSG3857);
        }
        else
        {
            source.SetType(mt::t_source::RELATIVE);
        }

        // проинициализируем высоту
        source.height = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 4)->widget())->value();

        // проверим, есть ли уже источник с такими координатами и высотой в set
        size_t id = std::hash<double>{}(source.height * x * y);
        dbt::t_sources::iterator sourceIter = sources.find(id);
        if (sourceIter == sources.end())
        {
            sourceIter = sources.insert(std::make_pair(id, source)).first;
        }

        // проверим, задан ли нуклид
        QComboBox* nuclideComboBox = qobject_cast<QComboBox*>(table->itemAtPosition(row, 1)->widget());
        if (nuclideComboBox->currentIndex() == 0)
        {
                MY_LOG("nuclide weren't chosen at source # " << row);
                return;
        }

        // создаем объект выброса, инициализируем индекс нуклида и температуру
        mt::t_emission emission;
        emission.nuclideName = nuclideComboBox->currentText();
        emission.temperature = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 5)->widget())->value();

        // инициализируем величину выброса
        if (UI->annualButton->isChecked())
        {
            double emissionValue = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 6)->widget())->value();
            if (!emissionValue)
            {
                MY_LOG("annual emission field is empty at source # " << row);
                return;
            }

            emission.value = mt::t_emissionValue(emissionValue);
        }
        else
        {
            double fisrtQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 7)->widget())->value();
            double secondQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 8)->widget())->value();
            double thirdQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 9)->widget())->value();
            double fourthQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 10)->widget())->value();

            if (!(fisrtQuarter && secondQuarter && thirdQuarter && fourthQuarter))
            {
                MY_LOG("quarterly emisiion fields are empty at source # " << row);
                return;
            }

            emission.value = mt::t_emissionValue(fisrtQuarter, secondQuarter, thirdQuarter, fourthQuarter);
        }

        // добавляем объект выброса в источник
        sourceIter->second.emissions.push_back(emission);
    }

    m_data->AddSources(sources);
}

void Connector::OnSourcesReset()
{
    m_data->OnSourcesReset();
}

void Connector::UpdateStartButton()
{
    bool ready = m_data->CheckStatus();
    UI->startButton->setEnabled(ready);
    UI->progressBar->setEnabled(ready);
    if (ready)
    {
        MY_LOG("ready to start calculation");
    }
}

#undef UI
