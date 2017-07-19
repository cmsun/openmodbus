#include <QApplication>
#include "modbusTest.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);   
    MasterTest test;
    test.show();
    return app.exec();
}
