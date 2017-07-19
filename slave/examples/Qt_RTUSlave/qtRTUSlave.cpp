#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSettings>
#include <QMessageBox>
#include "qtRTUSlave.h"
#include "mbSerial.h"

QtRTUSlave::QtRTUSlave(void)
{
    timer.setInterval(5);
    timer.setSingleShot(true);

    createWidgets();
    createConnects();
    initComboBox();
    readSettings();
}

QtRTUSlave::~QtRTUSlave(void)
{
    writeSettings(); 
}

void QtRTUSlave::createWidgets(void)
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

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(applyLayout);

    setLayout(mainLayout);
}

void QtRTUSlave::createConnects(void)
{
    connect(comComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setCom()));
    connect(baudRateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setBaudRate()));
    connect(dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataBits()));
    connect(parityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setParity()));
    connect(stopBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setStopBits()));
    connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(apply())); 

    connect(&serial, SIGNAL(readyRead()), this, SLOT(receive()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void QtRTUSlave::initComboBox(void)
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

void QtRTUSlave::writeSettings(void)
{
    QSettings settings("QtRTUSlave Inc.", "QtRTUSlave");
    settings.setValue("COM", comComboBox->currentText());
    settings.setValue("BaudRate", baudRateComboBox->currentText());
    settings.setValue("DataBits", dataBitsComboBox->currentText());
    settings.setValue("Parity", parityComboBox->currentText());
    settings.setValue("StopBits", stopBitsComboBox->currentText());
}

void QtRTUSlave::readSettings(void)
{
    int index;
    QSettings settings("QtRTUSlave Inc.", "QtRTUSlave");

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
}

void QtRTUSlave::apply(void)
{
    //close-------------------------------------------------------
    if(serial.isOpen())
    {
        serial.close();
        applyButton->setText("Apply");
        applyStatusLabel->setText("Plese open");    
        applyStatusLabel->setStyleSheet("background-color:transparent; color:red");
        return;
    }

    //open-------------------------------------------------------
    setCom();

    if (!serial.open(QIODevice::ReadWrite)) 
    {
        QMessageBox::warning(this, tr("QtRTUSlave"), 
                tr("Can't open %1, error code %2")
                .arg(serial.portName())
                .arg(serial.error()));
        goto err;
    }

    if (!serial.setFlowControl(QSerialPort::NoFlowControl)) 
    {
        QMessageBox::warning(this, tr("QtRTUSlave"),
                tr("Can't set no flow control to port %1, error code %2")
                .arg(serial.portName())
                .arg(serial.error()));
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

void QtRTUSlave::setCom(void)
{
    serial.close();
    serial.setPortName(comComboBox->currentText());
    applyStatusLabel->setText("Please open");    
    applyStatusLabel->setStyleSheet("color:red");
}

void QtRTUSlave::setBaudRate(void)
{
    if (!serial.setBaudRate(baudRateComboBox->currentText().toInt())) 
    {
        QMessageBox::warning(this, tr("QtRTUSlave"), 
                tr("Can't set rate %1 baud to port %2, error code %3")
                .arg(baudRateComboBox->currentText())
                .arg(serial.portName())
                .arg(serial.error()));
    }
}

void QtRTUSlave::setDataBits(void)
{
    QSerialPort::DataBits dataBits;
    dataBits = (QSerialPort::DataBits)dataBitsComboBox->currentText().toInt();
    if (!serial.setDataBits(dataBits))
    {
        QMessageBox::warning(this, tr("QtRTUSlave"),
                tr("Can't set %1 data bits to port %2, error code %3")
                .arg(dataBitsComboBox->currentText())
                .arg(serial.portName())
                .arg(serial.error()));
    }
}

void QtRTUSlave::setParity(void)
{
    QSerialPort::Parity parity;
    parity = (QSerialPort::Parity)parityComboBox->currentText().toInt();
    if (!serial.setParity(parity)) 
    {
        QMessageBox::warning(this, tr("QtRTUSlave"),
                tr("Can't set %1 parity to port %1, error code %2")
                .arg(parityComboBox->currentText())
                .arg(serial.portName())
                .arg(serial.error()));
    }
}

void QtRTUSlave::setStopBits(void)
{
    QSerialPort::StopBits stopBits;
    switch(stopBitsComboBox->currentIndex())
    {
        case 0: stopBits = QSerialPort::OneStop; break;
        case 1: stopBits = QSerialPort::OneAndHalfStop; break;
        case 2: stopBits = QSerialPort::TwoStop; break;
        default: stopBits = QSerialPort::UnknownStopBits;
    }

    if (!serial.setStopBits(stopBits)) 
    {
        QMessageBox::warning(this, tr("QtRTUSlave"), 
                tr("Can't set %1 stop bit to port %2, error code %3")
                .arg(stopBitsComboBox->currentText())
                .arg(serial.portName())
                .arg(serial.error()));
    }
}

void QtRTUSlave::receive(void)
{
    QByteArray byteArray = serial.readAll();
    foreach(const char byte, byteArray)
        mbSerialReceive(byte);
}

void QtRTUSlave::timeout(void)
{
    mbOnTimer();
}
