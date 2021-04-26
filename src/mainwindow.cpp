#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractSpinBox>
#include <QFileDialog>
#include <QString>
#include <QObject>
#include <QScrollBar>
#include <QComboBox>

#if !defined(MY_GREEN) && !defined(MY_RED)
#define MY_GREEN "005500"
#define MY_RED "aa0000"
#endif

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_row(2)
{
    m_ui->setupUi(this);
    CustomUiSettings();

    m_dir = QDir::current();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::UpdateClimateStatusLabel(const bool ok)
{
    UpdateStatusLabel(m_ui->climateStatusLabel, ok);
}

void MainWindow::UpdateGeoStatusLabel(const bool ok)
{
    UpdateStatusLabel(m_ui->geoStatusLabel, ok);
}

void MainWindow::UpdateImageStatusLabel(const bool ok)
{
    UpdateStatusLabel(m_ui->imageStatusLabel, ok);
}

void MainWindow::UpdateSourcesStatusLabel(const bool ok)
{
    UpdateStatusLabel(m_ui->srcStatusLabel, ok);
}

/**
 * @brief MainWindow::CustomUiSettings All the setups and connections within GUI only
 */
void MainWindow::CustomUiSettings()
{
    QApplication::setStyle("fusion");

    m_ui->youChoseGeoProfileLabel->setVisible(false);
    m_ui->progressBar->reset();

    connect(m_ui->imageResetButton, SIGNAL(clicked()), this, SLOT(ResetSpinBoxes()));

    connect(m_ui->climateBrowseButton, &QPushButton::clicked, this, [=](){ this->BrowseFile(m_ui->climateLineEdit, "*.csv"); });
    connect(m_ui->geoBrowseButton, &QPushButton::clicked, this, [=](){ this->BrowseFile(m_ui->geoLineEdit, "*.csv"); });
    connect(m_ui->imageBrowseButton, &QPushButton::clicked, this, [=](){ this->BrowseFile(m_ui->imageLineEdit, "*.jpg *.jpeg *.png"); });

    connect(m_ui->annualButton, &QRadioButton::toggled, this, &MainWindow::AnnualEmissionToggled);
    connect(m_ui->coordinatesEPSG4326RadioButton, &QRadioButton::toggled, this, [=](){ this->CoordinatesToggled(EPSG4326); });
    connect(m_ui->coordinatesEPSG3857RadioButton, &QRadioButton::toggled, this, [=](){ this->CoordinatesToggled(EPSG3857); });
    connect(m_ui->coordinatesRelativeRadioButton, &QRadioButton::toggled, this, [=](){ this->CoordinatesToggled(RELATIVE); });

    connect(m_ui->climateResetButton, &QPushButton::clicked, this, [=](){ this->UpdateStatusLabel(m_ui->climateStatusLabel, false); });
    connect(m_ui->geoResetButton, &QPushButton::clicked, this, [=](){ this->UpdateStatusLabel(m_ui->geoStatusLabel, false); });
    connect(m_ui->imageResetButton, &QPushButton::clicked, this, [=](){ this->UpdateStatusLabel(m_ui->imageStatusLabel, false); });
    connect(m_ui->srcResetButton, &QPushButton::clicked, this, &MainWindow::ResetSources);

    connect(m_ui->addSourceButrton, &QPushButton::clicked, this, &MainWindow::AddSource);
    connect(m_ui->removeSourceButton, &QPushButton::clicked, this, &MainWindow::RemoveSource);
}

void MainWindow::UpdateStatusLabel(QLabel *label, const bool result)
{
    if (result)
    {
        label->setStyleSheet(QString("QLabel { font-weight : bold; color : #%1; }").arg(MY_GREEN));
        label->setText("READY");
    }
    else
    {
        label->setStyleSheet(QString("QLabel { font-weight : bold; color : #%1; }").arg(MY_RED));
        label->setText("NOT READY");
    }
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
    QFileDialog dialog(this, tr("Open File"), m_dir.path(), filter);
    if (dialog.exec() == QFileDialog::Accepted)
    {
        m_dir = dialog.directory();
        QString filename = dialog.selectedFiles().first();
        lineEdit->setText(filename);
    }
}

void MainWindow::AnnualEmissionToggled(bool toggled)
{
    for (int row = 1; row < m_row; ++row)
    {
        m_ui->sourceTableLayout->itemAtPosition(row, 6)->widget()->setEnabled(toggled);
        m_ui->sourceTableLayout->itemAtPosition(row, 7)->widget()->setEnabled(!toggled);
        m_ui->sourceTableLayout->itemAtPosition(row, 8)->widget()->setEnabled(!toggled);
        m_ui->sourceTableLayout->itemAtPosition(row, 9)->widget()->setEnabled(!toggled);
        m_ui->sourceTableLayout->itemAtPosition(row, 10)->widget()->setEnabled(!toggled);
    }
}

void MainWindow::CoordinatesToggled(t_coordinatesType type)
{
    double firstMax;
    double secondMax;
    double firstMin;
    double secondMin;

    switch (type)
    {
    case EPSG4326:
        m_ui->xLabel->setText("lon, °");
        m_ui->yLabel->setText("lat, °");
        firstMax = 180;
        secondMax = 85;
        firstMin = -180;
        secondMin = -85;
        break;
    case EPSG3857:
        m_ui->xLabel->setText("E, m");
        m_ui->yLabel->setText("N, m");
        firstMax = 20037508.34;
        secondMax = 19971868.88;
        firstMin = -20037508.34;
        secondMin = -19971868.88;
        break;
    case RELATIVE:
        m_ui->xLabel->setText("x, %");
        m_ui->yLabel->setText("y, %");
        firstMax = 100;
        secondMax = 100;
        firstMin = 0;
        secondMin = 0;
        break;
    }

    for (int row = 1; row < m_row; ++row)
    {     
        qobject_cast<QDoubleSpinBox*>(m_ui->sourceTableLayout->itemAtPosition(row, 2)->widget())->setMaximum(firstMax);
        qobject_cast<QDoubleSpinBox*>(m_ui->sourceTableLayout->itemAtPosition(row, 3)->widget())->setMaximum(secondMax);
        qobject_cast<QDoubleSpinBox*>(m_ui->sourceTableLayout->itemAtPosition(row, 2)->widget())->setMinimum(firstMin);
        qobject_cast<QDoubleSpinBox*>(m_ui->sourceTableLayout->itemAtPosition(row, 3)->widget())->setMinimum(secondMin);
    }
}

void MainWindow::AddSource()
{
    QGridLayout* table = m_ui->sourceTableLayout;

    if (m_row > 500)
    {
        return;
    }

    // set label
    QLabel* number = new QLabel;
    number->setNum(m_row);
    table->addWidget(number, m_row, 0);

    // set nuclide
    QComboBox* nuclide = new QComboBox;
    QComboBox* prevNuclide = qobject_cast<QComboBox*>(table->itemAtPosition(m_row - 1, 1)->widget());
    for (int item = 0; item < prevNuclide->count(); ++item)
    {
        nuclide->addItem(prevNuclide->itemText(item));
    }
    nuclide->setCurrentIndex(prevNuclide->currentIndex());
    table->addWidget(nuclide, m_row, 1);

    // set spinboxes
    for (int column = 2; column < table->columnCount(); ++column)
    {
        QDoubleSpinBox* spinBox = new QDoubleSpinBox;
        QDoubleSpinBox* prev = qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(m_row - 1, column)->widget());
        spinBox->setMaximum(prev->maximum());
        spinBox->setMinimum(prev->minimum());
        spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox->setDecimals(prev->decimals());
        spinBox->setValue(prev->value());
        table->addWidget(spinBox, m_row, column);
    }

    m_row++;
    AnnualEmissionToggled(m_ui->annualButton->isChecked());

    // scroll down
    QScrollBar* scrollBar = m_ui->sourceScrollArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum()); // TODO почему то не до конца
}

void MainWindow::RemoveSource()
{
    QGridLayout* table = m_ui->sourceTableLayout;

    if (m_row < 3)
    {
        return;
    }

    for (int column = 0; column < table->columnCount(); ++column)
    {
        QLayoutItem* item = table->itemAtPosition(m_row - 1, column);
        QWidget* widget = item->widget();
        table->removeWidget(widget);
        table->removeItem(item);
        delete widget;
    }

    m_row--;
}

void MainWindow::ResetSources()
{
    for (int row = m_row - 1; row >= 2; --row)
    {
        RemoveSource();
    }

    QGridLayout* table = m_ui->sourceTableLayout;
    qobject_cast<QComboBox*>(table->itemAtPosition(1, 1)->widget())->setCurrentIndex(0);
    for (int column = 2; column < table->columnCount(); ++column)
    {
        qobject_cast<QDoubleSpinBox*>(table->itemAtPosition(1, column)->widget())->setValue(0);
    }

    UpdateStatusLabel(m_ui->srcStatusLabel, false);
}
