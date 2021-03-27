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

    connect(m_ui->srcCheckBox_2, &QCheckBox::toggled, this, &MainWindow::AddedSource2);
    connect(m_ui->srcCheckBox_3, &QCheckBox::toggled, this, &MainWindow::AddedSource3);
    connect(m_ui->srcCheckBox_4, &QCheckBox::toggled, this, &MainWindow::AddedSource4);
    connect(m_ui->srcCheckBox_5, &QCheckBox::toggled, this, &MainWindow::AddedSource5);

    connect(m_ui->annualButton, &QRadioButton::toggled, this, &MainWindow::AnnualEmissionToggled);
    connect(m_ui->quarterlyButton, &QRadioButton::toggled, this, &MainWindow::QuarterlyEmissionToggled);
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

void MainWindow::AddedSource2(bool toggled)
{
    m_ui->x2->setEnabled(toggled);
    m_ui->y2->setEnabled(toggled);
    m_ui->h2->setEnabled(toggled);
    m_ui->t2->setEnabled(toggled);
    m_ui->e2->setEnabled(toggled && m_ui->annualButton->isChecked());
    m_ui->q12->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q22->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q32->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q42->setEnabled(toggled && m_ui->quarterlyButton->isChecked());

    m_ui->srcCheckBox_3->setEnabled(toggled);
    if (!toggled)
    {
        m_ui->srcCheckBox_3->setChecked(toggled);
    }
}

void MainWindow::AddedSource3(bool toggled)
{
    m_ui->x3->setEnabled(toggled);
    m_ui->y3->setEnabled(toggled);
    m_ui->h3->setEnabled(toggled);
    m_ui->t3->setEnabled(toggled);
    m_ui->e3->setEnabled(toggled && m_ui->annualButton->isChecked());
    m_ui->q13->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q23->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q33->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q43->setEnabled(toggled && m_ui->quarterlyButton->isChecked());

    m_ui->srcCheckBox_4->setEnabled(toggled);
    if (!toggled)
    {
        m_ui->srcCheckBox_4->setChecked(toggled);
    }
}

void MainWindow::AddedSource4(bool toggled)
{
    m_ui->x4->setEnabled(toggled);
    m_ui->y4->setEnabled(toggled);
    m_ui->h4->setEnabled(toggled);
    m_ui->t4->setEnabled(toggled);
    m_ui->e4->setEnabled(toggled && m_ui->annualButton->isChecked());
    m_ui->q14->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q24->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q34->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q44->setEnabled(toggled && m_ui->quarterlyButton->isChecked());

    m_ui->srcCheckBox_5->setEnabled(toggled);
    if (!toggled)
    {
        m_ui->srcCheckBox_5->setChecked(toggled);
    }
}

void MainWindow::AddedSource5(bool toggled)
{
    m_ui->x5->setEnabled(toggled);
    m_ui->y5->setEnabled(toggled);
    m_ui->h5->setEnabled(toggled);
    m_ui->t5->setEnabled(toggled);
    m_ui->e5->setEnabled(toggled && m_ui->annualButton->isChecked());
    m_ui->q15->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q25->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q35->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
    m_ui->q45->setEnabled(toggled && m_ui->quarterlyButton->isChecked());
}

void MainWindow::AnnualEmissionToggled(bool toggled)
{
    m_ui->e1->setEnabled(toggled);
    m_ui->e2->setEnabled(toggled && m_ui->srcCheckBox_2->isChecked());
    m_ui->e3->setEnabled(toggled && m_ui->srcCheckBox_3->isChecked());
    m_ui->e4->setEnabled(toggled && m_ui->srcCheckBox_4->isChecked());
    m_ui->e5->setEnabled(toggled && m_ui->srcCheckBox_5->isChecked());
}

void MainWindow::QuarterlyEmissionToggled(bool toggled)
{
    m_ui->q11->setEnabled(toggled);
    m_ui->q21->setEnabled(toggled);
    m_ui->q31->setEnabled(toggled);
    m_ui->q41->setEnabled(toggled);

    m_ui->q12->setEnabled(toggled && m_ui->srcCheckBox_2->isChecked());
    m_ui->q22->setEnabled(toggled && m_ui->srcCheckBox_2->isChecked());
    m_ui->q32->setEnabled(toggled && m_ui->srcCheckBox_2->isChecked());
    m_ui->q42->setEnabled(toggled && m_ui->srcCheckBox_2->isChecked());

    m_ui->q13->setEnabled(toggled && m_ui->srcCheckBox_3->isChecked());
    m_ui->q23->setEnabled(toggled && m_ui->srcCheckBox_3->isChecked());
    m_ui->q33->setEnabled(toggled && m_ui->srcCheckBox_3->isChecked());
    m_ui->q43->setEnabled(toggled && m_ui->srcCheckBox_3->isChecked());

    m_ui->q14->setEnabled(toggled && m_ui->srcCheckBox_4->isChecked());
    m_ui->q24->setEnabled(toggled && m_ui->srcCheckBox_4->isChecked());
    m_ui->q34->setEnabled(toggled && m_ui->srcCheckBox_4->isChecked());
    m_ui->q44->setEnabled(toggled && m_ui->srcCheckBox_4->isChecked());

    m_ui->q15->setEnabled(toggled && m_ui->srcCheckBox_5->isChecked());
    m_ui->q25->setEnabled(toggled && m_ui->srcCheckBox_5->isChecked());
    m_ui->q35->setEnabled(toggled && m_ui->srcCheckBox_5->isChecked());
    m_ui->q45->setEnabled(toggled && m_ui->srcCheckBox_5->isChecked());
}
