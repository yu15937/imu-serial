#include "inc/mainwindow.h"
#include "inc/setdialog.h"
#include "ui_setdialog.h"

extern MainWindow *ptr;

setDialog::setDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setDialog)
{
    ui->setupUi(this);
    init();
    this->setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
}

setDialog::~setDialog()
{
    delete ui;
}

bool setDialog::isConnect(){
    return DataBase.db.isOpen();
}

void setDialog::init(){
    //设置为暗码
    ui->password->setEchoMode(QLineEdit::Password);
    //初始化
    openserial = false;
    //指示灯
    ui->lamp->setFixedSize(QSize(20,20));
    ui->lamp->setStyleSheet("QLabel{"
                            "  border-radius: 10px;" // 圆形标签
                            "  border: 1px solid black;" // 边框
                            "  background-color: white;" // 背景色
                            "}");
    onStyle = ui->lamp->styleSheet() + "QLabel {  background-color: green;}"; // 绿色背景表示开启
    offStyle = ui->lamp->styleSheet() + "QLabel {  background-color: red;}"; // 红色背景表示关闭
    ui->lamp->setStyleSheet(offStyle);

    //查找可用的串口
    const auto infos = QSerialPortInfo::availablePorts();
    //qDebug()<<"size:"<<infos.size();
    for(auto info:infos){
        QSerialPort serialTemp;
        serialTemp.setPort(info);
        if(serialTemp.open(QIODevice::ReadWrite)){
            portNames.append(info.portName());
            ui->SerialBox->addItem(info.portName());
            serialTemp.close();
        }
    }
    //波特率 默认为9600
    ui->BaudBox->setCurrentIndex(3);
}

int setDialog::autoInterval(){
    int interval = ui->interval->text().toInt();
    if(interval < 1){
        interval = 1;
    }
    return interval;
}

void setDialog::closeEvent(QCloseEvent *event)
{
    this->hide();
    ptr->dealSetDialogDestroy();
}

void setDialog::on_connect_clicked()
{
    username = ui->username->text();
    password = ui->password->text();
    //DataBase.connectDB(username,password);
    bool isconnect = DataBase.connectDB();

    if(isconnect){
        QMessageBox::warning(NULL,"提示","数据库连接成功");
        ui->labelConnect->setText("DataBase connect!");
        ui->labelConnect->setStyleSheet("color:black");
        QStringList DBTables = DataBase.db.tables();
//        for(int i=0;i<DBTables.size();i++){
//            QString tableName = DBTables.at(i);
//            qDebug()<<tableName;
//        }

        //初始化sqltablemodel
        ptr->dbviewdialog->sqlUserModel = new QSqlTableModel(this,ptr->setdialog->DataBase.db);
        ptr->dbviewdialog->sqlProdModel = new QSqlTableModel(this,ptr->setdialog->DataBase.db);
        ptr->dbviewdialog->init();

    }else{
        QMessageBox::warning(NULL,"提示","数据库连接失败");
        qDebug()<<"error open database because"<<db.lastError().text();
    }
}

void setDialog::on_disconnect_clicked()
{
    DataBase.disconnectDB();
    if(!DataBase.db.isOpen()){
        QMessageBox::warning(this,"提示","数据库已断开");
        QFont font;
        font.setPointSize(10);
        font.setFamily("Arial");
        ui->labelConnect->setFont(font);
        ui->labelConnect->setText("DataBase disconnect!");
        ui->labelConnect->setStyleSheet("color:red");
    }else{
        QMessageBox::warning(this,"提示","数据库断开失败");
    }
}

void setDialog::on_open_clicked()
{
    if(ui->open->text() == tr("打开串口")){
        ptr->serial = new QSerialPort;
        //设置串口名
        ptr->serial->setPortName(ui->SerialBox->currentText());
        //打开串口
        if(!ptr->serial->open(QIODevice::ReadWrite)){
            QMessageBox::warning(this,"提示","打开串口失败");
            return;
        }
        //设置波特率
        ptr->serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch (ui->BitNumBox->currentIndex()) {
            case 0:
                ptr->serial->setDataBits(QSerialPort::Data8);
                break;
            case 1:
                ptr->serial->setDataBits(QSerialPort::Data7);
                break;
            case 2:
                ptr->serial->setDataBits(QSerialPort::Data6);
                break;
            case 3:
                ptr->serial->setDataBits(QSerialPort::Data5);
                break;
            default:
                break;
        }
        //设置奇偶校验位
        switch (ui->ParityBox->currentIndex()) {
            case 0:
                ptr->serial->setParity(QSerialPort::NoParity);
                break;
            case 1:
                ptr->serial->setParity(QSerialPort::OddParity);
                break;
            case 2:
                ptr->serial->setParity(QSerialPort::EvenParity);
                break;
            default:
                break;
        }
        //设置停止位
        switch (ui->StopBox->currentIndex()) {
            case 0:
                ptr->serial->setStopBits(QSerialPort::OneStop);
                break;
            case 1:
                ptr->serial->setStopBits(QSerialPort::OneAndHalfStop);
                break;
            case 2:
                ptr->serial->setStopBits(QSerialPort::TwoStop);
                break;
            default:
                break;
        }
        //设置流控制
        ptr->serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭设置菜单控制
        ui->SerialBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);

        ui->open->setText(tr("关闭串口"));
        //连接信号槽
        connect(ptr->serial,SIGNAL(readyRead()),ptr,SLOT(handleData()));
        openserial = true;
        ui->lamp->setStyleSheet(onStyle);
        QFont font;
        font.setPointSize(10);
        font.setFamily("Arial");
        ui->labelPort->setFont(font);
        ui->labelPort->setText("Serial Port Open!");
        ui->labelPort->setStyleSheet("color:black;");
    }else{//关闭串口

        ptr->serial->clear();
        ptr->serial->close();
        ptr->serial->deleteLater();

        ui->SerialBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);

        ui->open->setText(tr("打开串口"));
        openserial = false;
        ui->lamp->setStyleSheet(offStyle);
        QFont font;
        font.setPointSize(10);
        font.setFamily("Arial");
        ui->labelPort->setFont(font);
        ui->labelPort->setText("Serial Port Close!");
        ui->labelPort->setStyleSheet("color:red;");
    }
    ptr->setScreen(openserial);
}

void setDialog::on_insert_clicked()
{
    QString name = ui->name->text();
    QString age = ui->age->text();
    if(!DataBase.db.isOpen()){
        QMessageBox::warning(this,"提示","数据库未连接!");
        return;
    }
    if(name == "" || age == ""){
        QMessageBox::warning(this,"提示","输入信息为空!");
    }else{
        DataBase.insertDB(name,age);
    }
}

void setDialog::on_del_clicked()
{
    QString name = ui->name->text();
    QString age = ui->age->text();
    if(!DataBase.db.isOpen()){
        QMessageBox::warning(this,"提示","数据库未连接!");
        return;
    }
    if(name == "" || age == ""){
        QMessageBox::warning(this,"提示","输入信息为空!");
    }else{
        DataBase.deleteDB(name);
    }
}

void setDialog::on_update_clicked()
{
    QString name = ui->name->text();
    QString age = ui->age->text();
    if(!DataBase.db.isOpen()){
        QMessageBox::warning(this,"提示","数据库未连接!");
        return;
    }
    if(name == "" || age == ""){
        QMessageBox::warning(this,"提示","输入信息为空!");
    }else{
        DataBase.updateDB(name,age);
    }
}

void setDialog::on_select_clicked()
{
    QString name = ui->name->text();
    QString age = ui->age->text();
    if(!DataBase.db.isOpen()){
        QMessageBox::warning(this,"提示","数据库未连接!");
        return;
    }
    if(name == "" || age == ""){
        QMessageBox::warning(this,"提示","输入信息为空!");
    }else{
        DataBase.selectDB(name);
    }
}

