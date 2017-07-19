#include <QApplication>
#include "qtRTUSlave.h"
#include "mbdata.h"

QtRTUSlave *modbus;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QtRTUSlave modbusRtu;
    modbus = &modbusRtu;
    modbusRtu.show();

    //往模拟量寄存器写入浮点数
    ((float *)mbAnalogIn)[0] = 0.123f;
    ((float *)mbAnalogIn)[1] = 3.0823f;
    ((float *)mbAnalogIn)[2] = 10023.123789f;

    return app.exec();
}

extern "C" {

void mbDisableTimer(void)
{
    modbus->timer.stop(); 
}

void mbEnableTimer(void)
{
    modbus->timer.start();    
}

void mbSerialSend(unsigned char *sendData, size_t length)
{
    modbus->serial.write((char *)sendData, length);
}

}


