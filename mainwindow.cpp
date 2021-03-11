#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    CustomUiSettings();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::CustomUiSettings()
{
    m_ui->youChoseGeoProfileLabel->setVisible(false);
    m_ui->progressBar->reset();

    connect(m_ui->imageResetButton, SIGNAL(clicked()), this, SLOT(ResetSpinBoxes()));
}

void MainWindow::ResetSpinBoxes()
{
    m_ui->northSpinBox->setValue(0);
    m_ui->eastSpinBox->setValue(0);
    m_ui->southSpinBox->setValue(0);
    m_ui->westSpinBox->setValue(0);
}
