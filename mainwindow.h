#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtGui>
#include <QMessageBox>
#include <QTableWidget>

#include "product.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTableWidget bookT;
    QTableWidget elecT;
    QTableWidget clothesT;
    QTableWidget foodT;

    void showGoods();//从内存显示商品列表
    bool readL();//读入四种商品列表到内存
    bool saveL();//写内存中四种商品的amount到数据库
    void readAndShowGoods(){if(readL())showGoods();}
    template <typename T>
    bool saveL(const QVector<T>&,QString tableName);
    QVector<Book> bookL;
    QVector<Elec> elecL;
    QVector<Clothes> clothesL;
    QVector<Food> foodL;
};

#endif // MAINWINDOW_H
