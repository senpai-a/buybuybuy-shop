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
#include "bankcard.h"

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
    void on_cartB_clicked();
    void removeFromCart(int index);
    void on_bankB_clicked();
    void readCards();
private:
    void showGoods();//从内存显示商品列表
    bool readL();//读入四种商品列表到内存
    bool saveL();//写内存中四种商品的amount到数据库
    void readAndShowGoods(){if(readL())showGoods();}
    bool saveCart();//保存购物车
    void readCart();

    template <typename T>
    bool saveL(const QVector<T>&,QString tableName);
    template <typename T>
    T* find(const QVector<T>&,int id);

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
    QVector<BankCard> cards;
    bool login;
};

#endif // MAINWINDOW_H
