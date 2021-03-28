#include "connector.h"
#include "ui_mainwindow.h"
#include "climatecsvparser.h"
#include "datainterface.h"

#include <QPushButton>

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

    // geospatial data
    connect(UI->geoAcceptButton, &QPushButton::clicked, this, &Connector::OnGeospatialAccept);
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

#undef UI
