#include <QFileDialog>
#include <QSignalMapper>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_CustomSetupUi();
    m_SetCustomUiConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::m_CustomSetupUi()
{
    ui->label_3->setVisible(false);
}

void MainWindow::m_SetCustomUiConnections()
{
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(m_BrowseFileHandlerClimate()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(m_BrowseFileHandlerGeo()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(m_BrowseFileHandlerImage()));
}

void MainWindow::m_BrowseFileHandler(QLineEdit* lineEdit, const QString& filter)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), filter);
    if (!filename.isNull())
    {
        lineEdit->setText(filename);
    }
}

void MainWindow::m_BrowseFileHandlerClimate()
{
    m_BrowseFileHandler(ui->lineEdit, "*csv");
}

void MainWindow::m_BrowseFileHandlerGeo()
{
    m_BrowseFileHandler(ui->lineEdit_2, "*csv");
}

void MainWindow::m_BrowseFileHandlerImage()
{
    m_BrowseFileHandler(ui->lineEdit_3, "*.jpg *.jpeg *.png *.bmp");
}

