#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtGui>
#include <QMessageBox>
#include <QTableWidget>

#include "product.h"
#include "user.h"
#include "cart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_userB_clicked();

    void on_addB_clicked();

private:
    void showGoods();//从内存显示商品列表
    bool readL();//读入四种商品列表到内存
    bool saveL();//写内存中四种商品的amount到数据库
    void readAndShowGoods(){if(readL())showGoods();}
    template <typename T>
    bool saveL(const QVector<T>&,QString tableName);

    Ui::MainWindow *ui;
    QTableWidget bookT;
    QTableWidget elecT;
    QTableWidget clothesT;
    QTableWidget foodT;    

    QVector<Book> bookL;
    QVector<Elec> elecL;
    QVector<Clothes> clothesL;
    QVector<Food> foodL;

    User u;
    Cart c;
    bool login;
};

#endif // MAINWINDOW_H
