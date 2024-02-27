#include "inc/dbviewdialog.h"
#include "inc/mainwindow.h"
#include "ui_dbviewdialog.h"

extern MainWindow *ptr;

DbViewDialog::DbViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbViewDialog)
{
    ui->setupUi(this);
    //设置左右调节页面
    ui->leftButton->setArrowType(Qt::LeftArrow);
    ui->rightButton->setArrowType(Qt::RightArrow);

    pageCnt = ui->stackedWidget->count();
    this->setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
}

void DbViewDialog::init(){
    tables.append(ui->userTable);
    tables.append(ui->prodTable);
    models.append(sqlUserModel);
    models.append(sqlProdModel);
}

DbViewDialog::~DbViewDialog()
{
    delete ui;
}

void DbViewDialog::closeEvent(QCloseEvent *event){
    this->hide();
    ptr->dealDBViewDialogDestroy();
}

//数据库信息展示
void DbViewDialog::showData(){
    if(ptr->setdialog->isConnect()){
        QString sqlQuery = "";
        QStringList DBTables = ptr->setdialog->DataBase.db.tables();
        for(int i=0;i<DBTables.size();i++){//获取所有表
            QString tableName = DBTables.at(i);
            //sqlModel->clear();
            models[i]->setTable(tableName);
            if(!models[i]->select()){
                QMessageBox::warning(this,"提示","查询失败!");
                return;
            }
            //调整自适应宽高
            tables[i]->resizeColumnsToContents();
            tables[i]->resizeRowsToContents();
            for(int j=0;j<models[i]->columnCount();j++){
                tables[i]->setColumnWidth(j,120);
            }

            //隔行变色
            tables[i]->setAlternatingRowColors(true);
            //使其不可编辑
            tables[i]->setEditTriggers(QAbstractItemView::NoEditTriggers);

            //设置
            if(tableName == "user"){
                models[i]->setHeaderData(models[i]->fieldIndex("id"),Qt::Horizontal,"编号");
                models[i]->setHeaderData(models[i]->fieldIndex("name"),Qt::Horizontal,"姓名");
                models[i]->setHeaderData(models[i]->fieldIndex("age"),Qt::Horizontal,"年龄");
            }else if(tableName == "prod"){
                models[i]->setHeaderData(models[i]->fieldIndex("id"),Qt::Horizontal,"编号");
                models[i]->setHeaderData(models[i]->fieldIndex("name"),Qt::Horizontal,"商品名称");
                models[i]->setHeaderData(models[i]->fieldIndex("price"),Qt::Horizontal,"价格");
            }

            tables[i]->setModel(models[i]);

            tables[i]->show();
        }
    }
}

void DbViewDialog::on_leftButton_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    if(index==0){
        ui->stackedWidget->setCurrentIndex(pageCnt-1);
    }else{
        ui->stackedWidget->setCurrentIndex(index-1);
    }
}


void DbViewDialog::on_rightButton_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    if(index==pageCnt-1){
        ui->stackedWidget->setCurrentIndex(0);
    }else{
        ui->stackedWidget->setCurrentIndex(index+1);
    }
}


void DbViewDialog::on_refresh_clicked()
{
    showData();
}

