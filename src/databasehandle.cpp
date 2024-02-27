#include "inc/databasehandle.h"

DataBaseHandle::DataBaseHandle()
{

}

//默认连接
bool DataBaseHandle::connectDB(){
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }else{
        db = QSqlDatabase::addDatabase("QODBC");
    }

    //设置Qt连接MySQL为自动重连
    //this->db.setConnectOptions("MYSQL_OPT_RECONNECT=1");

    //设置数据库的各项参数
    db.setPort(3306);
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("ycw19981018");
    db.setDatabaseName("list");

    bool isconnect = db.open();
    return isconnect;

}

void DataBaseHandle::connectDB(QString username, QString password){
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }else{
        db = QSqlDatabase::addDatabase("QODBC");
    }

    //设置Qt连接MySQL为自动重连
    //this->db.setConnectOptions("MYSQL_OPT_RECONNECT=1");

    //设置数据库的各项参数
    db.setPort(3306);
    db.setHostName("127.0.0.1");
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName("list");

    bool isconnect = db.open();
    if(isconnect){
        QMessageBox::warning(NULL,"提示","数据库连接成功");
    }else{
        QMessageBox::warning(NULL,"提示","数据库连接失败");
        qDebug()<<"error open database because"<<db.lastError().text();
    }
}

void DataBaseHandle::disconnectDB(){
    db.close();
}

void DataBaseHandle::transformDB(QString destDB){
    db.close();
    db.setDatabaseName(destDB);
    db.open();
}


void DataBaseHandle::insertDB(QString name, QString age){
    QString sqlQuery = "";
    //名字相同不能够插入,不需要exec
    sqlQuery = QString("SELECT * FROM user WHERE name = '%1';").arg(name);
    QSqlQuery query(sqlQuery);
    if(query.next()){
        QMessageBox::warning(NULL,"提示","数据已存在,插入失败");
        return;
    }

    sqlQuery = QString("INSERT INTO user ( name, age) "
                          "VALUES (:name, :age)");
    query.clear();
    query.prepare(sqlQuery);
    query.bindValue(":name",name);
    query.bindValue(":age",age);
    bool isInsert = query.exec();
    if(isInsert){
        QMessageBox::warning(NULL,"提示","数据插入成功");
    }else{
        QMessageBox::warning(NULL,"提示","数据插入失败");
        qDebug()<<query.lastError().text();
    }
}

void DataBaseHandle::deleteDB(QString name){
    QSqlQuery query;
    QString sqlQuery = "";
    sqlQuery = QString("SELECT * FROM user WHERE name = '%1';").arg(name);
    query.exec(sqlQuery);
    if(!query.next()){
        QMessageBox::warning(NULL,"提示","数据不存在,删除失败");
        return;
    }

    sqlQuery = QString("DELETE FROM user WHERE name = '%1'").arg(name);
    bool isDelete = query.exec(sqlQuery);
    if(isDelete){
        QMessageBox::warning(NULL,"提示","数据删除成功");
    }else{
        QMessageBox::warning(NULL,"提示","数据删除失败");
        qDebug()<<query.lastError().text();
    }
}

void DataBaseHandle::updateDB(QString name, QString age){
    QString sqlQuery = "";
    //名字相同不能够插入,不需要exec
    sqlQuery = QString("SELECT * FROM user WHERE name = '%1';").arg(name);
    QSqlQuery query(sqlQuery);
    if(!query.next()){
        QMessageBox::warning(NULL,"提示","数据不存在,更新失败");
        return;
    }

    sqlQuery = QString("UPDATE user SET age=%1 WHERE name = '%2';").arg(age).arg(name);
    bool isUpdate = query.exec(sqlQuery);
    if(isUpdate){
        QMessageBox::warning(NULL,"提示","数据更新成功");
    }else{
        QMessageBox::warning(NULL,"提示","数据更新失败");
        qDebug()<<query.lastError().text();
    }
}

void DataBaseHandle::selectDB(QString name){
    QString age = "";
    QString sqlQuery = "";
    //名字相同不能够插入,不需要exec
    sqlQuery = QString("SELECT * FROM user WHERE name = '%1';").arg(name);
    QSqlQuery query(sqlQuery);
    if(!query.next()){
        QMessageBox::warning(NULL,"提示","数据不存在");
        return;
    }else{
        age = query.value(2).toString();
    }
    QMessageBox::warning(NULL,"提示",QString("查询到信息:用户%1的年龄为%2").arg(name).arg(age));
}
