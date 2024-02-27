#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDataBase>
#include <QSerialPort>
#include <QTimer>
#include "inc/databasehandle.h"
#include "inc/setdialog.h"
#include "inc/dbviewdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dealSetDialogDestroy();
    void dealDBViewDialogDestroy();

    void setScreen(bool type);
    void parseData(QByteArray buf);

public:
    setDialog *setdialog;
    DbViewDialog *dbviewdialog;

    DataBaseHandle DataBase;//数据库操作
    QSqlDatabase db;//数据库

    QString username;
    QString password;

    QSerialPort *serial;
    QTimer timer;

public slots:
    void handleData();
    void writeData();

private slots:

    void on_actionSet_triggered();

    void on_clearButton_clicked();

    void on_sendButton_clicked();

    void on_actionDBView_triggered();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
