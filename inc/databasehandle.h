#ifndef DATABASEHANDLE_H
#define DATABASEHANDLE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


class DataBaseHandle
{

public:
    DataBaseHandle();

    //默认连接
    bool connectDB();
    void connectDB(QString username, QString password);
    void disconnectDB();
    void transformDB(QString destDB);

    //数据库操作
    void insertDB(QString name,QString age);
    void deleteDB(QString name);
    void updateDB(QString name,QString age);
    void selectDB(QString name);

public:
    QSqlDatabase db;//数据库

};

#endif // DATABASEHANDLE_H
