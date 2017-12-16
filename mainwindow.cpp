#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readAndShowGoods();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::readL(){
    bookL.clear();
    elecL.clear();
    clothesL.clear();
    foodL.clear();
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"获取商品列表失败","无法连接到数据库，请重试");
        return false;
    }
    QSqlQuery q;
    q.prepare("select ID,name,desc,amount,author,price from Book");
    if(!q.exec()){
        QMessageBox::information(this,"获取书籍类商品列表失败","无法查询数据库，请重试");
        return false;
    }
    while(q.next()){
        int id=q.value("ID").toInt();
        QString name=q.value("name").toString();
        QString desc=q.value("desc").toString();
        int amount=q.value("amount").toInt();
        QString author=q.value("author").toString();
        double price=q.value("price").toDouble();
        Book bk(id,name,desc,price,amount,author);
        bookL.push_back(bk);
    }

    q.prepare("select ID,name,desc,amount,brand,price from Elec");
    if(!q.exec()){
        QMessageBox::information(this,"获取电子类商品列表失败","无法查询数据库，请重试");
        return false;
    }
    while(q.next()){
        int id=q.value(0).toInt();
        QString name=q.value(1).toString();
        QString desc=q.value(2).toString();
        int amount=q.value(3).toInt();
        QString brand=q.value(4).toString();
        double price=q.value(5).toDouble();
        Elec elec(id,name,desc,price,amount,brand);
        elecL.push_back(elec);
    }

    q.prepare("select ID,name,desc,amount,sex,price from Clothes");
    if(!q.exec()){
        QMessageBox::information(this,"获取服装类商品列表失败","无法查询数据库，请重试");
        return false;
    }
    while(q.next()){
        int id=q.value(0).toInt();
        QString name=q.value(1).toString();
        QString desc=q.value(2).toString();
        int amount=q.value(3).toInt();
        int sexi=q.value(4).toInt();
        Clothes::Sex sex=sexi==0?
                    Clothes::General:
                    (sexi==1?Clothes::Male:Clothes::Female);
        double price=q.value(5).toDouble();
        Clothes c(id,name,desc,price,amount,sex);
        clothesL.push_back(c);
    }

    q.prepare("select ID,name,desc,amount,date,price from Food");
    if(!q.exec()){
        QMessageBox::information(this,"获取食品类商品列表失败","无法查询数据库，请重试");
        return false;
    }
    while(q.next()){
        int id=q.value(0).toInt();
        QString name=q.value(1).toString();
        QString desc=q.value(2).toString();
        int amount=q.value(3).toInt();
        QDate date=q.value(4).toDate();
        double price=q.value(5).toDouble();
        Food f(id,name,desc,price,amount,date);
        foodL.push_back(f);
    }
    return true;
}

template <typename T>
bool MainWindow::saveL(const QVector<T>& L, QString tableName){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"保存商品列表失败","无法连接到数据库，请重试");
        return false;
    }
    QSqlQuery q;
    for(auto it=L.begin();it!=L.end();it++){
        q.prepare("update ? set amount=? where ID=?");
        q.addBindValue(tableName);
        q.addBindValue(it->getAmount());
        q.addBindValue(it->getID());
        if(!q.exec()){
            QMessageBox::information(this,"保存书籍商品列表失败",
                                     QString("%1品类的一项保存失败").arg(tableName));
            return false;
        }
    }
    return true;
}

bool MainWindow::saveL(){
    if(saveL<Book>(bookL,"Book")&&saveL<Elec>(elecL,"Elec")
            &&saveL<Clothes>(clothesL,"Clothes")&&saveL<Food>(foodL,"Food"))
        return true;
    else return false;
}

void MainWindow::showGoods(){
    ui->tabWidget->clear();

    bookT.clear();
    bookT.setColumnCount(5);
    bookT.setRowCount(bookL.size());
    bookT.setHorizontalHeaderLabels(QStringList()
                                    <<"书名"
                                    <<"作者"
                                    <<"简介"
                                    <<"价格"
                                    <<"库存量"
                                    );
    for(auto it=bookL.begin();it!=bookL.end();it++){
        int i=it-bookL.begin();
        bookT.setItem(i,0,new QTableWidgetItem(it->getName()));
        bookT.setItem(i,1,new QTableWidgetItem(it->getAuthor()));
        bookT.setItem(i,2,new QTableWidgetItem(it->getDesc()));
        bookT.setItem(i,3,new QTableWidgetItem(QString("%1 元").arg(it->getPrice())));
        bookT.setItem(i,4,new QTableWidgetItem(QString("%1 本").arg(it->getAmount())));
        bookT.item(i,0)->setData(Qt::UserRole,it->getID());
    }

    elecT.clear();
    elecT.setColumnCount(5);
    elecT.setRowCount(bookL.size());
    elecT.setHorizontalHeaderLabels(QStringList()
                                    <<"品名"
                                    <<"品牌"
                                    <<"描述"
                                    <<"价格"
                                    <<"库存量"
                                    );
    for(auto it=elecL.begin();it!=elecL.end();it++){
        int i=it-elecL.begin();
        elecT.setItem(i,0,new QTableWidgetItem(it->getName()));
        elecT.setItem(i,1,new QTableWidgetItem(it->getBrand()));
        elecT.setItem(i,2,new QTableWidgetItem(it->getDesc()));
        elecT.setItem(i,3,new QTableWidgetItem(QString("%1 元").arg(it->getPrice())));
        elecT.setItem(i,4,new QTableWidgetItem(QString("%1 个").arg(it->getAmount())));
        elecT.item(i,0)->setData(Qt::UserRole,it->getID());
    }

    clothesT.clear();
    clothesT.setColumnCount(5);
    clothesT.setRowCount(bookL.size());
    clothesT.setHorizontalHeaderLabels(QStringList()
                                    <<"品名"
                                    <<"性别"
                                    <<"描述"
                                    <<"价格"
                                    <<"库存量"
                                    );
    QStringList sexL=QStringList()<<"通用"<<"男装"<<"女装";
    for(auto it=clothesL.begin();it!=clothesL.end();it++){
        int i=it-clothesL.begin();
        clothesT.setItem(i,0,new QTableWidgetItem(it->getName()));
        clothesT.setItem(i,1,new QTableWidgetItem(sexL[it->getSexi()]));
        clothesT.setItem(i,2,new QTableWidgetItem(it->getDesc()));
        clothesT.setItem(i,3,new QTableWidgetItem(QString("%1 元").arg(it->getPrice())));
        clothesT.setItem(i,4,new QTableWidgetItem(QString("%1 件").arg(it->getAmount())));
        clothesT.item(i,0)->setData(Qt::UserRole,it->getID());
    }

    foodT.clear();
    foodT.setColumnCount(5);
    foodT.setRowCount(bookL.size());
    foodT.setHorizontalHeaderLabels(QStringList()
                                    <<"名称"
                                    <<"描述"
                                    <<"价格"
                                    <<"生产日期"
                                    <<"库存量"
                                    );
    for(auto it=foodL.begin();it!=foodL.end();it++){
        int i=it-foodL.begin();
        foodT.setItem(i,0,new QTableWidgetItem(it->getName()));
        foodT.setItem(i,1,new QTableWidgetItem(it->getDesc()));
        foodT.setItem(i,2,new QTableWidgetItem(QString("%1 元").arg(it->getPrice())));
        foodT.setItem(i,3,new QTableWidgetItem(it->getDate().toString()));
        foodT.setItem(i,4,new QTableWidgetItem(QString("%1 个").arg(it->getAmount())));
        foodT.item(i,0)->setData(Qt::UserRole,it->getID());
    }

    bookT.setSelectionBehavior(QAbstractItemView::SelectRows);
    elecT.setSelectionBehavior(QAbstractItemView::SelectRows);
    clothesT.setSelectionBehavior(QAbstractItemView::SelectRows);
    foodT.setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabWidget->addTab(&bookT,QIcon(),"书籍");
    ui->tabWidget->addTab(&elecT,QIcon(),"电子");
    ui->tabWidget->addTab(&clothesT,QIcon(),"服装");
    ui->tabWidget->addTab(&foodT,QIcon(),"食品");
}
