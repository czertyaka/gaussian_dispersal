#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QString>
#include <QLabel>

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

public slots:
    void UpdateClimateStatusLabel(const bool ok);
    void UpdateGeoStatusLabel(const bool ok);
//    void UpdateImageStatusLabel(const bool ok);
//    void UpdateSourcesStatusLabel(const bool ok);

private slots:
    void ResetSpinBoxes();
    void BrowseFile(QLineEdit* lineEdit, const QString& file);
    void AddedSource2(bool);
    void AddedSource3(bool);
    void AddedSource4(bool);
    void AddedSource5(bool);
    void AnnualEmissionToggled(bool);
    void QuarterlyEmissionToggled(bool);
    void ResetSources();

private:
    void CustomUiSettings();
    void UpdateStatusLabel(QLabel* label, const bool ok);
    Ui::MainWindow* m_ui;
};
#endif // MAINWINDOW_H
