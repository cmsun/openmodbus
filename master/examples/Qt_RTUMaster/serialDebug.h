#ifndef SERIALDEBUG_H
#define SERIALDEBUG_H

#include <QWidget>

class QSerialPort;
class QComboBox;
class QPushButton;
class QRadioButton;
class QTextEdit;
class QLabel;

#include "qtRTUMaster.h"

class SerialDebug : public QWidget
{
    Q_OBJECT

private:
    QComboBox *comComboBox;
    QComboBox *baudRateComboBox;
    QComboBox *dataBitsComboBox;
    QComboBox *parityComboBox;
    QComboBox *stopBitsComboBox;
    QRadioButton *modbusRtuRadioBtn;
    QRadioButton *modbusAsciiRadioBtn;
    QRadioButton *hexRadioBtn;
    QRadioButton *stringRadioBtn;
    QRadioButton *ModbusRtuRadioBtn;
    QPushButton *applyButton;
    QLabel *applyStatusLabel;
    QPushButton *sendButton;
    QTextEdit *showEdit;
    QTextEdit *inputEdit;

    void createWidgets(void);
    void initComboBox(void);
    void createConnects(void);
    void writeSettings(void);
    void readSettings(void);

    QSerialPort serialPort;
    QSerialPort *serial;
    QByteArray responseData;

    QtRTUMaster modbusRtu;
    static void modbusRtuReply(const std::vector<mbu8> &sendData, 
            const std::vector<mbu8> &recData, void *arg, mbu8 err);

private slots:
    void apply(void);
    void setCom(void);
    void setBaudRate(void);
    void setDataBits(void);
    void setParity(void);
    void setStopBits(void);
    void protocolChange(void);
    void sendRequest(void);
    void receive(void);

protected:
    virtual void closeEvent(QCloseEvent *event);

public:
    SerialDebug(QWidget *parent = 0); 
};

#endif
