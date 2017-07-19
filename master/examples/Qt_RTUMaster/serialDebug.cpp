#include <QtSerialPort/QSerialPortInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>
#include <QTextEdit>
#include <QSettings>
#include <QMessageBox>
#include <QRegExp>

#include "serialDebug.h"
#include "mbRTUMaster.h"
#include "mbError.h"

SerialDebug::SerialDebug(QWidget *parent)
    : QWidget(parent)
    , serial(&serialPort)
{
    createWidgets();
    createConnects();
    initComboBox();
    readSettings();      //必须先调用initComboBox()再调用readSettings
}

void SerialDebug::closeEvent(QCloseEvent *event)
{
    writeSettings();
}

void SerialDebug::createWidgets(void)
{
    comComboBox = new QComboBox(this);
    baudRateComboBox = new QComboBox(this);
    dataBitsComboBox = new QComboBox(this);
    parityComboBox = new QComboBox(this);
    stopBitsComboBox = new QComboBox(this);

    QFormLayout *formLayout = new QFormLayout; 
    formLayout->setAlignment(Qt::AlignCenter);
    formLayout->addRow(tr("COM:"), comComboBox);
    formLayout->addRow(tr("BaudRate:"), baudRateComboBox);
    formLayout->addRow(tr("DataBits:"), dataBitsComboBox);
    formLayout->addRow(tr("Parity:"), parityComboBox);
    formLayout->addRow(tr("StopBits:"), stopBitsComboBox);
    
    applyButton = new QPushButton("Apply", this);
    applyStatusLabel = new QLabel(tr("Please open"), this);
    QHBoxLayout *applyLayout = new QHBoxLayout;
    applyLayout->addWidget(applyButton);
    applyLayout->addWidget(applyStatusLabel);

    modbusRtuRadioBtn = new QRadioButton(tr("Modbus RTU"), this);
    modbusAsciiRadioBtn = new QRadioButton(tr("Modbus ASCII"), this);
    hexRadioBtn = new QRadioButton(tr("Hex"), this);
    stringRadioBtn = new QRadioButton(tr("String"), this);
    sendButton = new QPushButton("Send", this);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(formLayout);
    leftLayout->addLayout(applyLayout);
    leftLayout->addStretch();
    leftLayout->addWidget(modbusRtuRadioBtn);
    leftLayout->addWidget(modbusAsciiRadioBtn);
    leftLayout->addWidget(hexRadioBtn);
    leftLayout->addWidget(stringRadioBtn);
    leftLayout->addWidget(sendButton);

    showEdit = new QTextEdit(this);
    inputEdit = new QTextEdit(this);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(showEdit);
    rightLayout->addWidget(inputEdit);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
}

void SerialDebug::createConnects(void)
{
    connect(comComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCom()));
    connect(baudRateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setBaudRate()));
    connect(dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataBits()));
    connect(parityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setParity()));
    connect(stopBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setStopBits()));
    connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(apply())); 
    connect(sendButton, SIGNAL(clicked(bool)), this, SLOT(sendRequest()));

    connect(modbusRtuRadioBtn, SIGNAL(clicked(bool)), this, SLOT(protocolChange()));
    connect(modbusAsciiRadioBtn, SIGNAL(clicked(bool)), this, SLOT(protocolChange()));
    connect(hexRadioBtn, SIGNAL(clicked(bool)), this, SLOT(protocolChange()));
    connect(stringRadioBtn, SIGNAL(clicked(bool)), this, SLOT(protocolChange()));

    connect(&serialPort, SIGNAL(readyRead()), this, SLOT(receive()));
}

void SerialDebug::initComboBox(void)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        comComboBox->addItem(info.portName());

    baudRateComboBox->addItems(QStringList()<<"1200"<<"2400"<<"9600"
                                            <<"19200"<<"38400"<<"57600"
                                            <<"115200");

    dataBitsComboBox->addItems(QStringList()<<"5"<<"6"<<"7"<<"8");

    parityComboBox->addItems(QStringList()<<"No"<<"Even"<<"Odd"<<"Mark");

    stopBitsComboBox->addItems(QStringList()<<"1"<<"1.5"<<"2");
}

void SerialDebug::writeSettings(void)
{
    QSettings settings("SerialDebug Inc.", "SerialDebug");
    settings.setValue("COM", comComboBox->currentText());
    settings.setValue("BaudRate", baudRateComboBox->currentText());
    settings.setValue("DataBits", dataBitsComboBox->currentText());
    settings.setValue("Parity", parityComboBox->currentText());
    settings.setValue("StopBits", stopBitsComboBox->currentText());

    int protocol;
    if(modbusRtuRadioBtn->isChecked())
        protocol = 0;
    else if(modbusAsciiRadioBtn->isChecked())
        protocol = 1;
    else if(hexRadioBtn->isChecked())
        protocol = 2;
    else
        protocol = 3;
    settings.setValue("protocol", protocol);
}

void SerialDebug::readSettings(void)
{
    int index;
    QSettings settings("SerialDebug Inc.", "SerialDebug");

    index = comComboBox->findText(settings.value("COM").toString()); 
    if(index != -1)
        comComboBox->setCurrentIndex(index);
    
    index = baudRateComboBox->findText(settings.value("BaudRate").toString()); 
    if(index != -1)
        baudRateComboBox->setCurrentIndex(index);

    index = dataBitsComboBox->findText(settings.value("DataBits").toString()); 
    if(index != -1)
        dataBitsComboBox->setCurrentIndex(index);

    index = parityComboBox->findText(settings.value("Parity").toString()); 
    if(index != -1)
        parityComboBox->setCurrentIndex(index);

    index = stopBitsComboBox->findText(settings.value("StopBits").toString()); 
    if(index != -1)
        stopBitsComboBox->setCurrentIndex(index);

    switch(settings.value("protocol").toInt())
    {
        case 0: modbusRtuRadioBtn->click(); break;
        case 1: modbusAsciiRadioBtn->click(); break;
        case 2: hexRadioBtn->click(); break;
        case 3: stringRadioBtn->click(); break;
        default: modbusRtuRadioBtn->click();
    }
}

void SerialDebug::apply(void)
{
    //close-------------------------------------------------------
    if(serial->isOpen())
    {
        serial->close();
        applyButton->setText("Apply");
        applyStatusLabel->setText("Plese open");    
        applyStatusLabel->setStyleSheet("background-color:transparent; color:red");
        return;
    }

    //open-------------------------------------------------------
    setCom();

    if (!serial->open(QIODevice::ReadWrite)) 
    {
        QMessageBox::warning(this, tr("SerialDebug"), 
                tr("Can't open %1, error code %2")
                .arg(serial->portName())
                .arg(serial->error()));
        goto err;
    }

    if (!serial->setFlowControl(QSerialPort::NoFlowControl)) 
    {
        QMessageBox::warning(this, tr("SerialDebug"),
                tr("Can't set no flow control to port %1, error code %2")
                .arg(serial->portName())
                .arg(serial->error()));
        goto err;
    }

    setBaudRate();
    setDataBits();
    setParity();
    setStopBits();

    applyButton->setText("Close");
    applyStatusLabel->setText("Open success");    
    applyStatusLabel->setStyleSheet("color:green");
    return;

err:
    applyButton->setText("Apply");
    applyStatusLabel->setText("Open failed");    
    applyStatusLabel->setStyleSheet("color:red");
}

void SerialDebug::setCom(void)
{
    serial->close();
    serial->setPortName(comComboBox->currentText());
    applyStatusLabel->setText("Please open");    
    applyStatusLabel->setStyleSheet("color:red");
}

void SerialDebug::setBaudRate(void)
{
    if (!serial->setBaudRate(baudRateComboBox->currentText().toInt())) 
    {
        QMessageBox::warning(this, tr("SerialDebug"), 
                tr("Can't set rate %1 baud to port %2, error code %3")
                .arg(baudRateComboBox->currentText())
                .arg(serial->portName())
                .arg(serial->error()));
    }
}

void SerialDebug::setDataBits(void)
{
    QSerialPort::DataBits dataBits;
    dataBits = (QSerialPort::DataBits)dataBitsComboBox->currentText().toInt();
    if (!serial->setDataBits(dataBits))
    {
        QMessageBox::warning(this, tr("SerialDebug"),
                tr("Can't set %1 data bits to port %2, error code %3")
                .arg(dataBitsComboBox->currentText())
                .arg(serial->portName())
                .arg(serial->error()));
    }
}

void SerialDebug::setParity(void)
{
    QSerialPort::Parity parity;
    parity = (QSerialPort::Parity)parityComboBox->currentText().toInt();
    if (!serial->setParity(parity)) 
    {
        QMessageBox::warning(this, tr("SerialDebug"),
                tr("Can't set %1 parity to port %1, error code %2")
                .arg(parityComboBox->currentText())
                .arg(serial->portName())
                .arg(serial->error()));
    }
}

void SerialDebug::setStopBits(void)
{
    QSerialPort::StopBits stopBits;
    switch(stopBitsComboBox->currentIndex())
    {
        case 0: stopBits = QSerialPort::OneStop; break;
        case 1: stopBits = QSerialPort::OneAndHalfStop; break;
        case 2: stopBits = QSerialPort::TwoStop; break;
        default: stopBits = QSerialPort::UnknownStopBits;
    }

    if (!serial->setStopBits(stopBits)) 
    {
        QMessageBox::warning(this, tr("SerialDebug"), 
                tr("Can't set %1 stop bit to port %2, error code %3")
                .arg(stopBitsComboBox->currentText())
                .arg(serial->portName())
                .arg(serial->error()));
    }
}

void SerialDebug::protocolChange(void)
{
    bool isOpen = serial->isOpen();

    if(isOpen)
        serial->close(); 

    if(modbusRtuRadioBtn->isChecked())
    {
        modbusRtu.setPort(QSerialPortInfo(*serial));
        serial = &modbusRtu;
    }
    else if(modbusAsciiRadioBtn->isChecked())
    {
        
    }
    else if(hexRadioBtn->isChecked() || stringRadioBtn->isChecked())
    {
        serialPort.setPort(QSerialPortInfo(*serial)); 
        serial = &serialPort;
    }

    if(isOpen)
        serial->open(QIODevice::ReadWrite);
}

void SerialDebug::sendRequest(void)
{
    if(modbusRtuRadioBtn->isChecked() || hexRadioBtn->isChecked())
    {
        QString str = inputEdit->toPlainText();
        QStringList strlist = str.split(" ");
        QByteArray byteArray;
        QRegExp regexp("[0-9A-Fa-f]{2}");
        foreach(const QString tmpstr, strlist)
        {
            if(!regexp.exactMatch(tmpstr))
            {
                showEdit->append("请输入16进制字节如：01 02 AA\n");
                return;
            }
            byteArray.append(tmpstr.toInt(NULL, 16));
        }

        if(modbusRtuRadioBtn->isChecked())
        {
            unsigned short crc = RTUMaster::computeCRC((unsigned char*)byteArray.data(), byteArray.size());
            byteArray.append(crc >> 8).append(crc & 0xff);
            modbusRtu.masterSend(std::vector<unsigned char>(byteArray.begin(), byteArray.end()),
                                modbusRtuReply,
                                this);
        }
        else
        {
            serial->write(byteArray);
        }
    }
    else if(stringRadioBtn->isChecked())
    {
        serial->write(inputEdit->toPlainText().toLocal8Bit());
    }
}

void SerialDebug::modbusRtuReply(const std::vector<mbu8> &sendData, 
        const std::vector<mbu8> &recData, void *arg, mbu8 err)
{
    SerialDebug &debuger = *(SerialDebug *)arg;

    QString receiveStr;
    foreach(const mbu8 &byte, recData)
        receiveStr += QString().sprintf("%02X ", byte);

    if(err != MODBUS_NO_ERROR)
        debuger.showEdit->append(tr("reply error: %1").arg(receiveStr));
    else
        debuger.showEdit->append(tr("reply: %1").arg(receiveStr));
}

void SerialDebug::receive(void)
{
    responseData.append(serial->readAll());
    showEdit->setText(QString(responseData));
}
