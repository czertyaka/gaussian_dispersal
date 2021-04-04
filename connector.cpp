#include "connector.h"
#include "ui_mainwindow.h"
#include "climatecsvparser.h"
#include "datainterface.h"
#include "sourcesdata.h"

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
    connect(m_data, &DataInterface::UpdateClimateStatusLabel, m_window, &MainWindow::UpdateClimateStatusLabel);

    // geospatial data
    connect(UI->geoAcceptButton, &QPushButton::clicked, this, &Connector::OnGeospatialAccept);
    connect(m_data, &DataInterface::UpdateGeoStatusLabel, m_window, &MainWindow::UpdateGeoStatusLabel);

    // sources
    connect(UI->srcAcceptButton, &QPushButton::clicked, this, &Connector::OnSourcesAccept);
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
            MY_LOG(__PRETTY_FUNCTION__ << ": filename field is empty");
        }
    }
    else
    {
        //TODO
    }
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
            MY_LOG(__PRETTY_FUNCTION__ << ": filename field is empty");
        }
    }
    else
    {
        //TODO
    }
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
            MY_LOG(__PRETTY_FUNCTION__ << ": nuclide weren't chosen at source # " << row);
            return;
        }
        else
        {
            source.nuclide = comboBox->currentText();
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
                MY_LOG(__PRETTY_FUNCTION__ << ": annual emission field is empty at aource # " << row);
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

            if (!fisrtQuarter && !secondQuarter && !thirdQuarter && !fourthQuarter)
            {
                MY_LOG(__PRETTY_FUNCTION__ << ": quarterly emisiion fields are empty at source # " << row);
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

#undef UI
