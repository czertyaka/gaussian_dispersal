#include "connector.h"
#include "ui_mainwindow.h"
#include "climatecsvparser.h"

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
    // climatic variables
    connect(UI->climateAcceptButton, &QPushButton::clicked, this, &Connector::OnClimateAccept);
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
    }
    else
    {
        //TODO load stuff from database
    }
}

#undef UI
