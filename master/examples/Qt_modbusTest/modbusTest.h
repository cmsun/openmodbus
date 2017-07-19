#ifndef MODBUSTEST_H
#define MODBUSTEST_H

#include <QWidget>
#include <stdlib.h>

#include "qtRTUMaster.h"

class QPushButton;
class QTextEdit;
class QVBoxLayout;

#define MODBUS_SLAVE_DEVID      1   //下位机设备地址
#define COIL_COUNT              32  
#define DIGITIN_COUNT           32
#define REGISTER_COUNT          32
#define ANALOGIN_COUNT          3

class MasterTest : public QWidget
{
    Q_OBJECT

private slots:
    void testRequest(void);
    void test_1_5(void);
    void test_3_6(void);
    void test_10(void);
    void test_2(void);
    void test_4(void);

public:
    MasterTest(QWidget *parent = NULL);
    QtRTUMaster modbus;
    QPushButton *button; 
    QTextEdit *edit;
    bool goOn;
    unsigned char byte;
    unsigned short word;
    std::vector<unsigned short> wordArray;

    static void testReply_1_5(const std::vector<mbu8> &sendData, const std::vector<mbu8> &recData, void *arg, mbu8 err);
    static void testReply_3_6(const std::vector<mbu8> &sendData, const std::vector<mbu8> &recData, void *arg, mbu8 err);
    static void testReply_10(const std::vector<mbu8> &sendData, const std::vector<mbu8> &recData, void *arg, mbu8 err);
    static void testReply_2(const std::vector<mbu8> &sendData, const std::vector<mbu8> &recData, void *arg, mbu8 err);
    static void testReply_4(const std::vector<mbu8> &sendData, const std::vector<mbu8> &recData, void *arg, mbu8 err);
};

#endif
