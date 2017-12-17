#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signindialog.h"
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login=false;
    ui->userLabel->setText("用户未登录");
    ui->userB->setText("登录/注册");
    ui->bankB->hide();
    ui->boughtB->hide();
    ui->addB->hide();
    ui->cartB->hide();
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
        bookT.item(i,3)->setData(Qt::UserRole,it->getPrice());
        bookT.item(i,4)->setData(Qt::UserRole,it->getAmount());
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
        elecT.item(i,3)->setData(Qt::UserRole,it->getPrice());
        elecT.item(i,4)->setData(Qt::UserRole,it->getAmount());
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
        clothesT.item(i,3)->setData(Qt::UserRole,it->getPrice());
        clothesT.item(i,4)->setData(Qt::UserRole,it->getAmount());
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
        foodT.item(i,2)->setData(Qt::UserRole,it->getPrice());
        foodT.item(i,4)->setData(Qt::UserRole,it->getAmount());
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

void MainWindow::on_userB_clicked()
{
    if(login){
        int comf=QMessageBox::question(this,"登出","确定退出登录吗？"
                              ,QMessageBox::Ok,QMessageBox::Cancel);
        if(comf!=QMessageBox::Ok){
            return;
        }
        u=User();
        c=Cart();
        login=false;
        ui->userLabel->setText("用户未登录");
        ui->userB->setText("登录/注册");
        ui->bankB->hide();
        ui->boughtB->hide();
        ui->addB->hide();
        ui->cartB->setText("我的购物车(0)");
        ui->cartB->hide();
        return;
    }

    SignInDialog dia(&u,this);
    dia.setWindowModality(Qt::ApplicationModal);
    dia.exec();
    if(dia.result()==QDialog::Accepted){
        login=true;
        ui->userLabel->setText(QString("欢迎光临，%1").arg(u.getName()));
        ui->userB->setText("退出登录");
        ui->bankB->show();
        ui->boughtB->show();
        ui->addB->show();
        ui->cartB->show();
        return;
    }
    else{
        u=User();
        c=Cart();
        login=false;
        ui->userLabel->setText("用户未登录");
        ui->userB->setText("登录/注册");
        return;
    }

}

void MainWindow::on_addB_clicked()
{
    QTableWidget* t=dynamic_cast<QTableWidget*>(ui->tabWidget->currentWidget());
    if(t==nullptr){
        QMessageBox::information(this,"FATAL ERROR"
                                 ,"mainwindow.cpp:on_addB_clicked():获取tabWidget当前页"
                                  "QTableWidget*转换失败");
        return;
    }
    if(t->currentItem()==nullptr){
        QMessageBox::information(this,"未选择商品","未选择商品，请重试");
        return;
    }
    int r=t->currentRow();
    int id=t->item(r,0)->data(Qt::UserRole).toInt();
    QString name=t->item(r,0)->text();
    int restAmount=t->item(r,4)->data(Qt::UserRole).toInt();
    if(restAmount<=0){
        QMessageBox::information(this,"库存不足！","抱歉，您选择的商品库存不足");
        return;
    }

    if(ui->tabWidget->currentIndex()==0){//Book
        QString author=t->item(r,1)->text();
        QString desc=t->item(r,2)->text();
        double price=t->item(r,3)->data(Qt::UserRole).toDouble();
        bool ask=false;
        int amount=QInputDialog::getInt(this,"输入购买数量","请输入购买数量："
                                        ,0,0,restAmount,1,&ask);
        if(amount==0||!ask)return;
        Book* b=new Book(id,name,desc,price,amount,author);
        c.addProduct(b);
    }
    else if(ui->tabWidget->currentIndex()==1){//Elec
        QString brand=t->item(r,1)->text();
        QString desc=t->item(r,2)->text();
        double price=t->item(r,3)->data(Qt::UserRole).toDouble();
        bool ask=false;
        int amount=QInputDialog::getInt(this,"输入购买数量","请输入购买数量："
                                        ,0,0,restAmount,1,&ask);
        if(amount==0||!ask)return;
        Elec* e=new Elec(id,name,desc,price,amount,brand);
        c.addProduct(e);
    }
    else if(ui->tabWidget->currentIndex()==2){//clothes
        QString sexs=t->item(r,1)->text();
        Clothes::Sex sex=Clothes::General;
        if(sexs==QString("通用"))sex=Clothes::General;
        else if(sexs==QString("男装"))sex=Clothes::Male;
        else if(sexs==QString("女装"))sex=Clothes::Female;

        QString desc=t->item(r,2)->text();
        double price=t->item(r,3)->data(Qt::UserRole).toDouble();
        bool ask=false;
        int amount=QInputDialog::getInt(this,"输入购买数量","请输入购买数量："
                                        ,0,0,restAmount,1,&ask);
        if(amount==0||!ask)return;
        Clothes* clo=new Clothes(id,name,desc,price,amount,sex);
        c.addProduct(clo);
    }
    else if(ui->tabWidget->currentIndex()==3){//Food
        QString desc=t->item(r,1)->text();
        double price=t->item(r,2)->data(Qt::UserRole).toDouble();
        QDate date=QDate::fromString(t->item(r,3)->text());
        bool ask=false;
        int amount=QInputDialog::getInt(this,"输入购买数量","请输入购买数量："
                                        ,0,0,restAmount,1,&ask);
        if(amount==0||!ask)return;
        Food* f=new Food(id,name,desc,price,amount,date);
        c.addProduct(f);
    }
    ui->cartB->setText(QString("我的购物车(%1)").arg(c.getSize()));
    return;
}

void MainWindow::on_cartB_clicked()
{

}
