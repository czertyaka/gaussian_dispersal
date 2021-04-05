#include "connector.h"
#include "ui_mainwindow.h"
#include "climatecsvparser.h"
#include "datainterface.h"
#include "sourcesdata.h"
#include "imagedata.h"

#include <QPushButton>
#include <QGridLayout>
#include <QObject>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>

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

    // geospatial data
    connect(UI->geoAcceptButton, &QPushButton::clicked, this, &Connector::OnGeospatialAccept);
    connect(UI->geoResetButton, &QPushButton::clicked, this, &Connector::OnGeospatialReset);
    connect(UI->geoAcceptButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(UI->geoResetButton, &QPushButton::clicked, this, &Connector::UpdateStartButton);
    connect(m_data, &DataInterface::UpdateGeoStatusLabel, m_window, &MainWindow::UpdateGeoStatusLabel);

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
}

void Connector::OnClimateAccept()
{
    if (UI->climateLoadRadioButton->isChecked())
    {
        QString filename = UI->climateLineEdit->text();
        ClimateCsvParser::t_format format = ClimateCsvParser::RP5;
        if (!filename.isEmpty())
        {
            m_data->AddClimaticJournal(filename, format);
        }
        else
        {
            MY_LOG(": climate filename field is empty");
        }
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
        if (!filename.isEmpty())
        {
            m_data->AddGeospatialData(filename);
        }
        else
        {
            MY_LOG(": geospatial data filename field is empty");
        }
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
    ImageData::t_optBorders optBorders;

    if (UI->imageLoadRadioButton->isChecked())
    {
        filename = UI->imageLineEdit->text();
        if (filename.isEmpty())
        {
            MY_LOG(": image filename field is empty");
            return;
        }
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
    SourcesData::t_vSources vSources;
    QGridLayout* table = UI->sourceTableLayout;

    for (int row = 1; row < m_window->SourcesRows(); ++row)
    {
        SourcesData::t_source source;
        source.id = qobject_cast<QLabel*>(table->itemAtPosition(row, 0)->widget())->text().toInt();

        QComboBox* comboBox = qobject_cast<QComboBox*>(table->itemAtPosition(row, 1)->widget());
        if (comboBox->currentIndex() == 0)
        {
            MY_LOG(": nuclide weren't chosen at source # " << row);
            return;
        }
        else
        {
            source.nuclide = comboBox->currentText();
        }

        if (UI->coordinatesEPSGRadioButton->isChecked())
        {
            source.coordinatesType = SourcesData::EPSG3857;
        }
        else
        {
            source.coordinatesType = SourcesData::RELATIVE;
        }

        source.x = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 2)->widget())->value();
        source.y = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 3)->widget())->value();
        source.height = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 4)->widget())->value();
        source.temp = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 5)->widget())->value();

        if (UI->annualButton->isChecked())
        {
            double emission = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 6)->widget())->value();
            if (!emission)
            {
                MY_LOG(": annual emission field is empty at aource # " << row);
                return;
            }

            source.emission = emission;
        }
        else
        {
            double fisrtQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 7)->widget())->value();
            double secondQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 8)->widget())->value();
            double thirdQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 9)->widget())->value();
            double fourthQuarter = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(row, 10)->widget())->value();

            if (!(fisrtQuarter && secondQuarter && thirdQuarter && fourthQuarter))
            {
                MY_LOG(": quarterly emisiion fields are empty at source # " << row);
                return;
            }

            source.firstQuarter = fisrtQuarter;
            source.secondQuarter = secondQuarter;
            source.fourthQuarter = thirdQuarter;
            source.fourthQuarter = fourthQuarter;
        }

        vSources.push_back(source);
    }

    m_data->AddSources(vSources);
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
        MY_LOG(": ready to start calculation");
    }
}

#undef UI
