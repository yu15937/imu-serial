#ifndef DBVIEWDIALOG_H
#define DBVIEWDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QSqlTableModel>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class DbViewDialog;
}

class DbViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DbViewDialog(QWidget *parent = nullptr);
    ~DbViewDialog();

    void showData();
    void closeEvent(QCloseEvent *event);

public:
    int pageCnt = 0;
    QStandardItemModel *userModel;
    QStandardItemModel *prodModel;
    QSqlTableModel *sqlUserModel;
    QSqlTableModel *sqlProdModel;

    QVector<QSqlTableModel *> models;
    QVector<QTableView*> tables;//table类型

public slots:
    void init();

private slots:

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_refresh_clicked();

private:
    Ui::DbViewDialog *ui;
};

#endif // DBVIEWDIALOG_H
