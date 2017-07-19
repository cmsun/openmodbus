#ifndef QTRTUSLAVE_H
#define QTRTUSLAVE_H

#include <QtSerialPort/QSerialPort>
#include <QWidget>
#include <QTimer>

class QComboBox;
class QPushButton;
class QRadioButton;
class QLabel;

class QtRTUSlave : public QWidget
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

    void createWidgets(void);
    void createConnects(void);
    void initComboBox(void);
    void writeSettings(void);
    void readSettings(void);

private slots:
    void apply(void);
    void setCom(void);
    void setBaudRate(void);
    void setDataBits(void);
    void setParity(void);
    void setStopBits(void);
    void timeout(void);
    void receive(void);

public:
    QtRTUSlave(void);
    ~QtRTUSlave(void);
    QSerialPort serial;
    QTimer timer; 
};

#endif
