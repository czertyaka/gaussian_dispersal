#include <QFileDialog>
#include <QSignalMapper>

#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_climateTab(nullptr)
{
    ui->setupUi(this);
    m_CustomSetupUi();
    m_SetCustomUiConnections();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
    if (m_climateTab)
    {
        delete m_climateTab;
    }
}

/**
 * @brief MainWindow::m_CustomSetupUi GUI setups which couldn't be implemented via Designer
 */
void MainWindow::m_CustomSetupUi()
{
    ui->label_3->setVisible(false);
}

/**
 * @brief MainWindow::m_SetCustomUiConnections Sets connection too complicate for implemenattion in Designer
 */
void MainWindow::m_SetCustomUiConnections()
{
    // climate tab
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){ this->m_BrowseFileHandler(ui->lineEdit, "*csv"); });
    connect(ui->pushButton_7, &QPushButton::clicked, ui->comboBox, [=](){ ui->comboBox->setCurrentIndex(0); });
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::m_ClimateAcceptButtonHandler);

    // geospatial data tab
    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){ this->m_BrowseFileHandler(ui->lineEdit_2, "*csv"); });
    connect(ui->pushButton_10, &QPushButton::clicked, ui->comboBox_2, [=](){ ui->comboBox_2->setCurrentIndex(0); });
    connect(ui->pushButton_10, &QPushButton::clicked, ui->comboBox_3, [=](){ ui->comboBox_3->setCurrentIndex(0); });

    // background image tab
    connect(ui->pushButton_5, &QPushButton::clicked, this, [=](){ this->m_BrowseFileHandler(ui->lineEdit_3, "*.jpg *.jpeg *.png *.bmp"); });
    connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::m_ResetSpinBoxes);
}

/**
 * @brief MainWindow::m_BrowseFileHandler Opens a file dialog and puts the filename string to lineEdit
 * @param lineEdit QLineEdit object to put filename string in
 * @param filter Rule to filter the files in dialog
 */
void MainWindow::m_BrowseFileHandler(QLineEdit* lineEdit, const QString& filter)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), filter);
    if (!filename.isNull())
    {
        lineEdit->setText(filename);
    }
}

/**
 * @brief MainWindow::m_ResetSpinBoxes Resets spinboxes for image boundaries
 */
void MainWindow::m_ResetSpinBoxes()
{
    ui->doubleSpinBox->setValue(0);
    ui->doubleSpinBox_2->setValue(0);
    ui->doubleSpinBox_3->setValue(0);
    ui->doubleSpinBox_4->setValue(0);
}

void MainWindow::m_StartButtonHandler()
{
    if (m_climateTab && m_climateTab->GetStatus() == tabStatus_t::READY)
    {}
}

void MainWindow::m_ClimateAcceptButtonHandler()
{
    if (!m_climateTab)
    {
        m_climateTab = new ClimateTabManager;
    }

    try
    {
        if (ui->radioButton->isChecked())
        {
            m_climateTab->Init(ui->lineEdit->text());
        }
        else
        {
            ClimatePresets_t preset = static_cast<ClimatePresets_t>(ui->comboBox->currentIndex());
            m_climateTab->Init(preset);
        }
    }
    catch(...) //TODO
    {

    }
}
