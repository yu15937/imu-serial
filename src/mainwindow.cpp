#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *ptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);
    ptr = this;
    setdialog = new setDialog();
    dbviewdialog = new DbViewDialog();
    //不可编辑
    ui->receiveScreen->setReadOnly(true);
    QObject::connect(&timer, &QTimer::timeout, this, &MainWindow::writeData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dealSetDialogDestroy(){
    ui->actionSet->setEnabled(true);
}

void MainWindow::dealDBViewDialogDestroy(){
    ui->actionDBView->setEnabled(true);
}

//Imu串口数据处理逻辑
void MainWindow::handleData(){
    QByteArray buf = ptr->serial->readAll();
    if(!buf.isEmpty()){
        QString str = ui->receiveScreen->toPlainText();
        str += tr(buf);
        ui->receiveScreen->clear();
        ui->receiveScreen->append(str);
        parseData(buf);
    }
    buf.clear();
}

//Imu通讯协议逻辑
/*
 * IMU标定数据库需记录IMU序列号和标定参数
 * IMU标定数据库下发标定参数至电路板
*/
void MainWindow::parseData(QByteArray buf){
    int size = buf.size();
    QVector<int> pos;
    //获取二进制消息头
    for(int i=0;i<size-1;i++){
        if(buf.at(i) == 0x51 && buf.at(i+1) == 0x58){
            //qDebug()<<i;
            pos.append(i);
        }
    }
    //加上最后一个消息索引
    pos.append(size);
    //判断消息类型
    for(int i=0;i<pos.size()-1;i++){
        int startIndex = pos.at(i);
        int endIndex = pos.at(i+1);
        QByteArray data = buf.mid(startIndex,endIndex-startIndex);
        char msgclass = data.at(2);
        if(msgclass != 0x02){
            continue;
        }
        char msgID = data.at(3);
        int length = data.mid(4,2).toHex().toInt();
        if(msgID == 0x01){//IMU序列  1/60Hz

            QByteArray payLoad = data.mid(6,length);
            char ImuType = payLoad.at(0);
            char channel = payLoad.at(1);
            char subChannel = payLoad.at(2);
            QByteArray serialNum = payLoad.mid(3,length - 3);

            //qDebug()<<"ImuType:"<<ImuType<<",channel:"<<channel<<",subChannel:"<<subChannel<<",serialNum:"<<serialNum.toHex();

        }else if(msgID == 0x02){//IMU原始观测数据  100Hz

            QByteArray payLoad = data.mid(6,length);
            int msTime = payLoad.mid(0,4).toHex().toInt();
            char channel = payLoad.at(4);
            char subChannel = payLoad.at(5);
            int usTime = payLoad.mid(6,2).toHex().toInt();

            QByteArray senData = payLoad.mid(8,length - 8);
            int gyroX = 0, gyroY = 0, gyroZ = 0;
            int accelX = 0, accelY = 0, accelZ = 0, temp = 0;

            int typeMark = 0x3C000000;
            int dataMark = 0x00FFFFFF;

            for(int j=0;j<senData.size();j+=4){//每四个字节为一个有效数据
                unsigned int subData = senData.mid(j,4).toUInt();
                int type = subData & typeMark;
                if(type == 0x05){//gyroZ
                    gyroZ = subData & dataMark;
                }else if(type == 0x0D){//gyroY
                    gyroY = subData & dataMark;
                }else if(type == 0x0E){//gyroZ
                    gyroX = subData & dataMark;
                }else if(type == 0x10){//acclZ
                    accelZ = subData & dataMark;
                }else if(type == 0x11){//acclY
                    accelY = subData & dataMark;
                }else if(type == 0x12){//acclX
                    accelX = subData & dataMark;
                }else if(type == 0x0C){//temp
                    temp = subData & dataMark;
                }
            }


        }else if(msgID == 0x03){//IMU EEPROM数据 1/60Hz

            QByteArray payLoad = data.mid(6,length);
            char type = payLoad.at(0);
            char channel = payLoad.at(1);
            char subChannel = payLoad.at(2);
            QByteArray carParams = payLoad.mid(3,length - 3);

            //数据库操作

            if(type == 0x00){//上传至上位机

            }else if(type == 0x01){//下发至EEPROM

            }

        }else{
            /* do nothing */
        }
    }
}

void MainWindow::writeData(){
    QString sendText = ui->sendScreen->toPlainText();
    QByteArray sendByte = sendText.toUtf8();
    serial->write(QByteArray::fromHex(sendByte));
}

void MainWindow::setScreen(bool type){
    ui->sendButton->setEnabled(type);
}

void MainWindow::on_actionSet_triggered()
{
    ui->actionSet->setEnabled(false);
    setdialog->show();
}

void MainWindow::on_clearButton_clicked()
{
    ui->receiveScreen->clear();
}


void MainWindow::on_sendButton_clicked()
{
    if(ui->checkAuto->isChecked()){
        timer.setInterval(setdialog->autoInterval());
        timer.start();
    }else{
        writeData();
    }
}

void MainWindow::on_actionDBView_triggered()
{
    ui->actionDBView->setEnabled(false);
    dbviewdialog->show();
}

void MainWindow::on_stopButton_clicked()
{
    timer.stop();
}

