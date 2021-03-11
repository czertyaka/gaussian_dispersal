#include "connector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    DataInterface data;
    Connector connector(window, data);
    return a.exec();
}
