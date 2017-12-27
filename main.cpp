#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QtSql>
#include <QtGui>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./shop.db3");
    if(!db.open()){
        QMessageBox::information(0,"失败","连接到数据库失败，请检查数据库文件后重试启动。");
        return 0;
    }
    qDebug()<<db.tables();
    MainWindow w;
    w.show();

    return a.exec();
}
