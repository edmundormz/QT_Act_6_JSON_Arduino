#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conectarArduino();
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
        arduino->waitForReadyRead(3000);
        QByteArray datosLeidos = arduino->readAll();
        qInfo() << "Entrada: " + datosLeidos;

        QJsonDocument jsonA = QJsonDocument::fromJson(datosLeidos);
        QJsonObject SerialR = jsonA.object();
    }
}
