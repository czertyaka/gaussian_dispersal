#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

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
    Ui::MainWindow *ui;
    void m_CustomSetupUi();
    void m_SetCustomUiConnections();

private slots:
    void m_BrowseFileHandler(QLineEdit* lineEdit, const QString& filter);
    void m_ResetSpinBoxes();
};
#endif // MAINWINDOW_H
