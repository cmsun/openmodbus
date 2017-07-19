#include <QApplication>
#include "serialDebug.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SerialDebug modbus;
    modbus.show(); 

    return app.exec();
}
