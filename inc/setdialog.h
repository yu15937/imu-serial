#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "inc/databasehandle.h"

namespace Ui {
class setDialog;
}

class setDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setDialog(QWidget *parent = nullptr);
    ~setDialog();

    bool isConnect();
    void init();
    void closeEvent(QCloseEvent *event);
    int autoInterval();

public:
    QList<QString> portNames;

    DataBaseHandle DataBase;//数据库操作
    QSqlDatabase db;//数据库

    QString username;
    QString password;

    QString onStyle;
    QString offStyle;


    bool openserial;

private slots:

    void on_connect_clicked();

    void on_disconnect_clicked();

    void on_open_clicked();

    void on_insert_clicked();

    void on_del_clicked();

    void on_update_clicked();

    void on_select_clicked();

signals:
    void ready();

private:
    Ui::setDialog *ui;
};

#endif // SETDIALOG_H
