#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QEventLoop>
#include <time.h>

#include "mbError.h"
#include "modbusTest.h"

MasterTest::MasterTest(QWidget *parent)
    : QWidget(parent)
{
    button = new QPushButton("开始测试", this);
    edit = new QTextEdit;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(edit);
    layout->addWidget(button);
    setLayout(layout);

    modbus.setPortName(QString("COM4"));

    if (!modbus.open(QIODevice::ReadWrite)) {
        edit->append("open error\n");
    }

    if (!modbus.setBaudRate(QSerialPort::Baud115200)) {
        edit->append("setBaudRate error\n");
    }

    if (!modbus.setDataBits(QSerialPort::Data8)) {
        edit->append("setDataBits error\n");
    }

    if (!modbus.setParity(QSerialPort::NoParity)) {
        edit->append("setParity error");
    }

    if (!modbus.setStopBits(QSerialPort::OneStop)) {
        edit->append("setStopBits error");
    }

    if (!modbus.setFlowControl(QSerialPort::NoFlowControl)) {
        edit->append("setFlowControl error");
    }

    connect(button, SIGNAL(clicked()), this, SLOT(testRequest()));
}

void MasterTest::testRequest(void)
{
    test_1_5();
    test_3_6();
    test_10();
    test_2();
    test_4();    
}

void MasterTest::test_1_5(void)
{
    QEventLoop eventLoop;

    srand(time(NULL));

    for(int i = 0; i < COIL_COUNT-8; ++i)
    {
        eventLoop.processEvents();
        byte = rand(); 
        for(int j = 0; j < 8; ++j)
        {
            modbus.masterFun05(
                    MODBUS_SLAVE_DEVID, 
                    i+j, 
                    byte & (0x01<<j) ? 0xff : 0x00, 
                    NULL, 
                    NULL);
        }
        modbus.masterFun01(MODBUS_SLAVE_DEVID, i, 8, testReply_1_5, this);
       goOn = false;
        while(!goOn)
        {
            eventLoop.processEvents(); 
        }
    }
}

void MasterTest::testReply_1_5(const std::vector<mbu8> &sendData, 
        const std::vector<mbu8> &recData, void *arg, mbu8 err)
{
    MasterTest *test = (MasterTest *)arg;

    if(err != MODBUS_NO_ERROR) 
    {
        test->edit->append(QString("test1_5, Error when read address %1, err code:%2\n")
                                   .arg(QString::number(sendData[2]<<8|sendData[3], 16))
                                   .arg(QString::number(err, 16)));
    }
    else
    {
        unsigned char byte = recData[3];
        test->edit->append( QString().sprintf("test_1_5, %s, address:%04x, send:%04x, receive:%04x\n",
                    test->byte == byte ? "success" : "failed",
                    sendData[2]<<8|sendData[3],
                    test->byte,
                    byte) );

    }

    test->goOn = true;
}

void MasterTest::test_3_6(void)
{
    QEventLoop eventLoop;

    srand(time(NULL));

    for(int i = 0; i < REGISTER_COUNT; ++i)    
    {
        eventLoop.processEvents();
        word = rand();
        modbus.masterFun06(MODBUS_SLAVE_DEVID, i, word, NULL, NULL);        
        modbus.masterFun03(MODBUS_SLAVE_DEVID, i, 1, testReply_3_6, this);        
        goOn = false;
        while(!goOn)
        {
            eventLoop.processEvents();
        }
    }
}

void MasterTest::testReply_3_6(const std::vector<mbu8> &sendData, 
        const std::vector<mbu8> &recData, void *arg, mbu8 err)
{
    MasterTest *test = (MasterTest *)arg;

    if(err != MODBUS_NO_ERROR) 
    {
        test->edit->append(QString("test_3_6, Error when read address %1, err code:%2\n")
                            .arg(QString::number(sendData[2]<<8|sendData[3], 16))
                            .arg(QString::number(err, 16)));
    }
    else
    {
        unsigned short word = recData[3]<< 8 | recData[4];
        test->edit->append( QString().sprintf("test_3_6, %s, address:%04x, send:%04x, receive:%04x\n",
                                              test->word == word ? "success" : "failed",
                                              sendData[2]<<8|sendData[3],
                                              test->word,
                                              word) );
    }

    test->goOn = true;        
}


void MasterTest::test_10(void)
{
    wordArray.clear();
    wordArray.reserve(REGISTER_COUNT);

    srand(time(NULL));
    for(int i = 0; i < REGISTER_COUNT; ++i)
        wordArray.push_back(rand());

    int main = REGISTER_COUNT / 5;
    int remain = REGISTER_COUNT % 5;
    for(int i = 0; i < main; ++i)
    {
        unsigned short *arrayPtr = &wordArray[0] + i*5;
        modbus.masterFun10(MODBUS_SLAVE_DEVID, i*5, std::vector<mbu16>(arrayPtr, arrayPtr+5), NULL, NULL);        
    }
    if(remain > 0)
        modbus.masterFun10(MODBUS_SLAVE_DEVID, 
                REGISTER_COUNT-remain, 
                std::vector<mbu16>(&wordArray.back()-remain+1, &wordArray.back()+1), 
                NULL,
                NULL);        

    for(int i = 0; i < main; ++i)
        modbus.masterFun03(MODBUS_SLAVE_DEVID, i*5, 5, testReply_10, this);        
    if(remain > 0)
        modbus.masterFun03(MODBUS_SLAVE_DEVID, REGISTER_COUNT-remain, remain, testReply_10, this);
}

void MasterTest::testReply_10(const std::vector<mbu8> &sendData, 
        const std::vector<mbu8> &recData, void *arg, mbu8 err)
{
    static int time = 0;
    int main = REGISTER_COUNT / 5;
    int remain = REGISTER_COUNT % 5;
    QString sendStr, recStr;

    MasterTest *test = (MasterTest *)arg;

    if(err != MODBUS_NO_ERROR) 
    {
        test->edit->append(QString("test_10, Error when read address %1, err code:%2\n")
                            .arg(QString::number(sendData[2]<<8|sendData[3], 16))
                            .arg(QString::number(err, 16)));
    }
    else
    {
        bool equal = true;
        int count = (time < main ? 5 : remain);
        for(int i = 0; i < count; ++i)
        {
            unsigned short sendValue = test->wordArray[time*5+i];
            unsigned short recValue = recData[3+i*2]<<8 | recData[4+i*2];
            if(sendValue != recValue)
                equal = false;
            sendStr += QString().sprintf("%04x ", sendValue);
            recStr += QString().sprintf("%04x ", recValue);
        }

        test->edit->append( QString().sprintf("test_10, %s, address:%04x, send:%s, receive:%s",
                            equal ? "success" : "failed",
                            sendData[2]<<8|sendData[3],
                            sendStr.toStdString().c_str(),
                            recStr.toStdString().c_str()) );
    }    

    time++;
    if(remain == 0 && time == main)
        time = 0;
    else if(time > main)
        time = 0;
}

void MasterTest::test_2(void)
{
    for(int i = 0; i < DIGITIN_COUNT/8; ++i) 
        modbus.masterFun02(MODBUS_SLAVE_DEVID, i*8, 8, testReply_2, this);
}

void MasterTest::testReply_2(const std::vector<mbu8> &sendData, 
        const std::vector<mbu8> &recData, void *arg, mbu8 err)
{
    MasterTest *test = (MasterTest *)arg;
    QString msg;

    if(err != MODBUS_NO_ERROR) 
    {
        test->edit->append(QString("test_2, Error when read address %1, err code:%2\n")
                            .arg(QString::number(sendData[2]<<8|sendData[3], 16))
                            .arg(QString::number(err, 16)));
    }
    else
    {
        msg = QString().sprintf("test_2, address:%04x, value:%02x\n", sendData[2]<<8|sendData[3], recData[3]);
        test->edit->append(msg);
    }    
}

//功能码4浮点数读取测试
typedef union{
    float f;
    unsigned int u;
} floatint_t;

void MasterTest::test_4(void)
{
    for(int i = 0; i < ANALOGIN_COUNT*2; i += 2)    
        modbus.masterFun04(MODBUS_SLAVE_DEVID, i, 2, testReply_4, this);
}

void MasterTest::testReply_4(const std::vector<mbu8> &sendData, 
        const std::vector<mbu8> &recData, void *arg, mbu8 err)
{
    MasterTest *test = (MasterTest *)arg;
    QString msg;

    if(err != MODBUS_NO_ERROR) 
    {
        test->edit->append(QString("test_4, Error when read address %1, err code:%2\n")
                            .arg(QString::number(sendData[2]<<8|sendData[3], 16))
                            .arg(QString::number(err, 16)));
    }
    else
    {
        float f = RTUMaster::formatToFloat_regRever(recData[3], recData[4], recData[5], recData[6]); 
        // float f = RTUMaster::formatToFloat_noRever(recData[3], recData[4], recData[5], recData[6]); 
        msg = QString().sprintf("%02x, %02x, %02x, %02x\n", recData[3], recData[4], recData[5], recData[6]);
        msg += QString().sprintf("test_4, address:%04x, value:%.6f\n", sendData[2]<<8|sendData[3], f);
        test->edit->append(msg);
    }
}
