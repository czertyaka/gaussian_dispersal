#include <QFileDialog>
#include <QSignalMapper>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifndef DB_CONNECTION
#define DB_CONNECTION "QSQLITE"
#endif /* _DB_CONNECTION_NAME */

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete m_ui;
}
