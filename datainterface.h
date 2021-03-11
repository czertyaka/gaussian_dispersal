#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>

class DataInterface : public QObject
{
    Q_OBJECT
public:
    explicit DataInterface(QObject *parent = nullptr);

signals:

};

#endif // DATAINTERFACE_H
