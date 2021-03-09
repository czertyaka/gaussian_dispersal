#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

#include "climatetabmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class. Also implements custom connections and GUI setups
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void m_CustomSetupUi();
    void m_SetCustomUiConnections();

private slots:

    // common
    void m_BrowseFileHandler(QLineEdit* lineEdit, const QString& filter);
    void m_StartButtonHandler();
    void m_ClimateAcceptButtonHandler();

    // background image tab
    void m_ResetSpinBoxes();

private:
    Ui::MainWindow *ui;
    ClimateTabManager* m_climateTab;
};
#endif // MAINWINDOW_H
