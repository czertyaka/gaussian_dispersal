#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow * Ui() { return m_ui; }

private slots:
    void ResetSpinBoxes();
    void BrowseFile(QLineEdit* lineEdit, const QString& file);
    void AddedSource2(bool);
    void AddedSource3(bool);
    void AddedSource4(bool);
    void AddedSource5(bool);
    void AnnualEmissionToggled(bool);
    void QuarterlyEmissionToggled(bool);

private:
    void CustomUiSettings();
    Ui::MainWindow* m_ui;
};
#endif // MAINWINDOW_H
