#include "connector.h"
#include "database.h"
#include "ui_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    DataInterface& data = DataInterface::GetInstance();
    Connector connector(window, data);

    if (!DataBase::GetInstance().Init())
    {
        window.Ui()->tabs->setDisabled(true);
    }

    window.UpdateNuclides();

    return a.exec();
}
