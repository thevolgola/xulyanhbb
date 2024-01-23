#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <connectiondialog.h>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer =new QTimer(this);
    COMPORT = new QSerialPort;
    COMPORT -> setPortName("COM4");
    COMPORT -> setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT -> setParity(QSerialPort::Parity::NoParity);
    COMPORT -> setDataBits(QSerialPort::DataBits::Data8);
    COMPORT -> setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT -> setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT -> open(QIODevice::ReadWrite);


    ui->comboBox->addItem("Red");
    ui->comboBox->addItem("Blue");
    ui->comboBox->addItem("Green");
    ui->comboBox->addItem("Yellow");
    ui->comboBox_2->addItem("Red");
    ui->comboBox_2->addItem("Blue");
    ui->comboBox_2->addItem("Green");
    ui->comboBox_2->addItem("Yellow");



    connect(timer, SIGNAL(timeout()), this, SLOT(timerfunc()));
    timer->setInterval(1000);
    timer->start();




}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionConnection_triggered()
{
    connectionDialog connectiondialog;
    connectiondialog.setModal(true);
    connectiondialog.exec();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index2)
{
    qDebug() << index2;
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << index;
}

void MainWindow::on_actionStart_triggered()
{

    QString ip = ui->ip->text();
    string ip2 = ip.toStdString();
    cv::VideoCapture cap(ip2);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    while (true){
        cv::Mat frame;
        cap.read(frame);
        cap >> frame;

        cv::Mat hsv_frame;
        cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
        cv::Scalar lower_blue(100, 150, 0), upper_blue(140, 255, 255);
        cv::Scalar lower_red(0, 70, 50), upper_red(10, 255, 255);
        cv::Scalar lower_green(35, 70, 50), upper_green(85, 255, 255);
        cv::Scalar lower_yellow(20, 100, 100), upper_yellow(30, 255, 255);

        // Tạo mặt nạ cho từng màu
        cv::Mat mask_blue, mask_red, mask_green, mask_yellow;
        cv::inRange(hsv_frame, lower_blue, upper_blue, mask_blue);
        cv::inRange(hsv_frame, lower_red, upper_red, mask_red);
        cv::inRange(hsv_frame, lower_green, upper_green, mask_green);
        cv::inRange(hsv_frame, lower_yellow, upper_yellow, mask_yellow);

        // Tính tổng số pixel cho từng màu
        int total_blue = cv::countNonZero(mask_blue);
        int total_red = cv::countNonZero(mask_red);
        int total_green = cv::countNonZero(mask_green);
        int total_yellow = cv::countNonZero(mask_yellow);

        // So sánh tổng số pixel để xác định màu chính
        if (total_blue > total_red && total_blue > total_green && total_blue > total_yellow){
            code = 1;
        color = "Blue";
        }
        else if (total_red > total_blue && total_red > total_green && total_red > total_yellow){
            code = 0;
        color = "Red";
        }
        else if (total_green > total_blue && total_green > total_red && total_green > total_yellow){
            code = 2;
        color = "Green";
        }
        else if (total_yellow > total_blue && total_yellow > total_red && total_yellow > total_green){
            code = 3;
        color = "Yellow";
        }
        else
            code = 4;

        if(ui->comboBox->currentIndex() != ui->comboBox_2->currentIndex()){
            if(ui->comboBox->currentIndex() == code){
                servo = "180";
                updateServo(servo);

            }
            if(ui->comboBox_2->currentIndex()==code){
                servo="0";
                updateServo(servo);

            }

        }

        int height = frame.rows;
        int width = frame.cols;

        int cx = width / 2;
        int cy = height / 2;

        cv::Vec3b pixel_center = hsv_frame.at<cv::Vec3b>(cy, cx);



        std::cout << pixel_center << std::endl;

        cv::putText(frame, color, cv::Point(18, 58), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2);
        cv::circle(frame, cv::Point(cx, cy), 5, cv::Scalar(255, 0, 0), 3);
        cv::imshow("frame", frame);
        char key = cv::waitKey(1);
        if (key == 27) {
            break;
        }


    }


    cap.release();
    destroyAllWindows();
    COMPORT -> close();



}




void MainWindow::on_pushButton_clicked()
{
    QFile file("G:/20207761/opencvtest/xulyanh/ip address.txt");
    if(file.open(QIODevice::WriteOnly| QIODevice::Append| QIODevice::Text)) {
        QTextStream out(&file);
        QString ip = ui->ip->text();
        out << ip << " " << "\n";
        file.flush();
        file.close();
    }


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Could not open file"));
        return;
    }
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");

        for (int i = 0; i < fields.size(); ++i) {

            ui->comboBox_3->addItem(line);
        }
    }
    file.close();


}

void MainWindow::timerfunc()
{
    ui->label_timer->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy   HH:mm:ss"));
}

void MainWindow::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    QString ip1 = arg1;
    ui->ip->setText(ip1);
}

void MainWindow::updateServo(QString cmd)
{
    if(COMPORT->isWritable())
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateServo(QString)));
                if(COMPORT->isWritable())
                {
                    qDebug() << cmd.toStdString().c_str();
                    COMPORT->write(cmd.toStdString().c_str());
                }
                else
                {
                    qDebug()<<"coundn't write to serial";
                }
            ;
        timer->start(1000); // Truyền tín hiệu mỗi 1 giây
    }

}
















