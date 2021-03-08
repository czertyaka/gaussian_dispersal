#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void m_BrowseFileHandler(QLineEdit* lineEdit, const QString& filter);

private slots:
    void m_BrowseFileHandlerClimate();
    void m_BrowseFileHandlerGeo();
    void m_BrowseFileHandlerImage();
};
#endif // MAINWINDOW_H
