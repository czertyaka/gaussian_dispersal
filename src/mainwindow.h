#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QString>
#include <QLabel>
#include <QDir>

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
    int SourcesRows() { return m_row; }

private:
    enum t_coordinatesType
    {
        EPSG3857,
        EPSG4326,
        RELATIVE
    };

public slots:
    void UpdateClimateStatusLabel(const bool ok);
    void UpdateGeoStatusLabel(const bool ok);
    void UpdateImageStatusLabel(const bool ok);
    void UpdateSourcesStatusLabel(const bool ok);

    void OnClimateDone(const bool ok);

private slots:
    void ResetSpinBoxes();
    void BrowseFile(QLineEdit* lineEdit, const QString& file);
    void AnnualEmissionToggled(bool);
    void CoordinatesToggled(t_coordinatesType);
    void AddSource();
    void RemoveSource();
    void ResetSources();

private:
    void CustomUiSettings();
    void UpdateStatusLabel(QLabel* label, const bool ok);

    Ui::MainWindow* m_ui;
    int m_row; ///< rows number of the sources table
    QDir m_dir; ///< directory which will open in file dialog
};
#endif // MAINWINDOW_H
