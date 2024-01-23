#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QWidget>
#include <QFile>
#include <connectiondialog.h>
#include <iostream>
#include <sstream>
#include <opencv2\imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_actionConnection_triggered();

    void on_actionStart_triggered();

    void timerfunc();

    void on_pushButton_clicked();
    void updateServo(QString cmd);


    void on_comboBox_3_currentTextChanged(const QString &arg1);



    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QSerialPort* COMPORT;
    QTimer *timer;
    QTimer *timers;
    QString ip;
    int code;
    int hue_value;
    int hue_value2;
    int hue_value3;
    int count;
    string color;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendorID = 6790;
    static const quint16 arduino_uno_productID = 29987;
    QString arduino_port_name;
    bool arduino_is_available;
    QString defaulteagle;
    QString Data_from_serialPort;
    bool IS_Data_Reveived=false;
    QString servo;
    string imgpath;
    Mat frame;
    atomic<bool>newframeavailable;




};
#endif // MAINWINDOW_H
