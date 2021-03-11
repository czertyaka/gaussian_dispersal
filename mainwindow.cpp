#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

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

/**
 * @brief MainWindow::CustomUiSettings All the setups and connections within GUI only
 */
void MainWindow::CustomUiSettings()
{
    m_ui->youChoseGeoProfileLabel->setVisible(false);
    m_ui->progressBar->reset();

    connect(m_ui->imageResetButton, SIGNAL(clicked()), this, SLOT(ResetSpinBoxes()));

    connect(m_ui->climateBrowseButton, &QPushButton::clicked, this, [=](){ this->BrowseFile(m_ui->climateLineEdit, "*.csv"); });
    connect(m_ui->geoBrowseButton, &QPushButton::clicked, this, [=](){ this->BrowseFile(m_ui->geoLineEdit, "*.csv"); });
    connect(m_ui->imageBrowseButton, &QPushButton::clicked, this, [=](){ this->BrowseFile(m_ui->imageLineEdit, "*.jpg *.jpeg *.png"); });
}

/**
 * @brief MainWindow::ResetSpinBoxes Resets SpinBoxes at background image tab
 */
void MainWindow::ResetSpinBoxes()
{
    m_ui->northSpinBox->setValue(0);
    m_ui->eastSpinBox->setValue(0);
    m_ui->southSpinBox->setValue(0);
    m_ui->westSpinBox->setValue(0);
}

/**
 * @brief MainWindow::BrowseFile Finds a file and puts its name in QLineEdit
 * @param lineEdit
 * @param filter
 */
void MainWindow::BrowseFile(QLineEdit* lineEdit, const QString& filter)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), filter);
    if (!filename.isNull())
    {
        lineEdit->setText(filename);
    }
}
