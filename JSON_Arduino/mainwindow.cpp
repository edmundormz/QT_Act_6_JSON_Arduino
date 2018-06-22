#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtNetwork>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdN1->setFrameStyle(QFrame::NoFrame);
    ui->lcdN2->setFrameStyle(QFrame::NoFrame);
    conectarArduino();
    sendJson();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::conectarArduino(){

    //Parte # 1, declaración inicial de las variables
    arduino_esta_conectado = false;
    arduino = new QSerialPort(this);
    connect(arduino, &QSerialPort::readyRead, this, &MainWindow::recepcionSerialAsyncrona);
    QString nombreDispositivoSerial = "";
    int nombreProductID = 0;

    //-2 buscar puertos con los identificadores de Arduino
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Identificador del fabricante (VENDOR ID): " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "ID Vendedor " << serialPortInfo.vendorIdentifier();
            qDebug() << "ID Producto: " << serialPortInfo.productIdentifier();

            if(serialPortInfo.productIdentifier() == 24577 || serialPortInfo.productIdentifier() == 67){
                arduino_esta_conectado = true;
                nombreDispositivoSerial = serialPortInfo.portName();
                nombreProductID = serialPortInfo.productIdentifier();
            }
        }
    }

    //3-Conexion
    if(arduino_esta_conectado){
        ui->lbPortName->setText(nombreDispositivoSerial);
        arduino ->setPortName(nombreDispositivoSerial);
        arduino->open(QIODevice::ReadWrite);
        arduino ->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        //arduino->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "Producto: " << nombreProductID;
//        if(nombreProductID == 24577) ui->label->setText("Arduino Black conectado");
//        else if(nombreProductID ==66) ui->label->setText("Arduino Mega conectado");
    }
}

void MainWindow::recepcionSerialAsyncrona(){
    if(arduino_esta_conectado && arduino->isReadable()){
        int Bytes = arduino->bytesAvailable();
        qInfo() << "DatosDisp: " + QString::number(Bytes);
        //arduino->waitForReadyRead(3000);
        QByteArray datosLeidos = arduino->readAll();
        qInfo() << "Entrada: " + datosLeidos;
        ui->lbReceived->setText(datosLeidos);

        QJsonDocument jsonA = QJsonDocument::fromJson(datosLeidos);
        QJsonObject SerialR = jsonA.object();

        QJsonValue ADC1 = SerialR.value("ADC1");
        double value_ADC1 = ADC1.toDouble();
        ui->lcdN1->display(5*value_ADC1/1024);

        QJsonValue ADC2 = SerialR.value("ADC2");
        double value_ADC2 = ADC2.toDouble();
        ui->lcdN2->display(5*value_ADC2/1024);
    }
}

void MainWindow::sendJson(){
    cadenaJSON = "{\"LED1\":\"" + LED1 + "\",\"LED2\":\"" + LED2 + "\",\"ADC1\":\"" + ADC1 + "\",\"ADC2\":\"" + ADC2 +"\"}" ;
    ui->lbSent->setText(cadenaJSON);
    if(arduino_esta_conectado && arduino->isWritable()){
        arduino->write(cadenaJSON.toUtf8().constData());//This may work to act5
        qDebug() << "Salida:" << cadenaJSON.toUtf8().constData();
        QThread::msleep(200);
    }
}

void MainWindow::on_comboBoxLed1_currentIndexChanged(int index)
{
    if(index == 0){
        LED1 = "0";
    }
    else if(index == 1){
        LED1 = "1";
    }
    qDebug() << "LED1 " << LED1;
    sendJson();
}

void MainWindow::on_comboBoxLed2_currentIndexChanged(int index)
{
    if(index == 0){
        LED2 = "0";
    }
    else if(index == 1){
        LED2 = "1";
    }
    qDebug() << "LED2 " << LED2;
    sendJson();
}

