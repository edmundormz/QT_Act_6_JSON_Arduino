#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBoxLed1_currentIndexChanged(int index);

    void on_comboBoxLed2_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;

    bool arduino_esta_conectado = false;
    void conectarArduino();
    void recepcionSerialAsyncrona();
    void sendJson();

    QString cadenaJSON = "";
    QString LED1 = "0";
    QString LED2 = "0";
    QString ADC1 = "0";
    QString ADC2 = "0";
};

#endif // MAINWINDOW_H
