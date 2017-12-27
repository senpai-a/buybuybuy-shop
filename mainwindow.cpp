#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signindialog.h"
#include "cartdialog.h"
#include "bankdialog.h"
#include "paydialog.h"
#include "boughtdialog.h"
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
    bookT.setEditTriggers(QTableWidget::NoEditTriggers);
    elecT.setEditTriggers(QTableWidget::NoEditTriggers);
    clothesT.setEditTriggers(QTableWidget::NoEditTriggers);
    foodT.setEditTriggers(QTableWidget::NoEditTriggers);
    bookT.setSelectionBehavior(QAbstractItemView::SelectRows);
    elecT.setSelectionBehavior(QAbstractItemView::SelectRows);
    clothesT.setSelectionBehavior(QAbstractItemView::SelectRows);
    foodT.setSelectionBehavior(QAbstractItemView::SelectRows);
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
        QDate date=QDate::fromString(q.value(4).toString());
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
        q.prepare(QString("update %1 set amount=? where ID=?").arg(tableName));
        //q.addBindValue(tableName);
        q.addBindValue(it->getAmount());
        q.addBindValue(it->getID());
        //QMessageBox::information(0,"update",QString("update \'%1\' set amount=%2 where ID=%3")
        //              .arg(tableName).arg(it->getAmount()).arg(it->getID()));
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
    elecT.setRowCount(elecL.size());
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
    clothesT.setRowCount(clothesL.size());
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
    foodT.setRowCount(foodL.size());
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

template <typename T>
T* MainWindow::find(const QVector<T>& L,int id){
    for(auto it=L.begin();it!=L.end();it++){
        if(it->getID()==id)
            return (T*)it;
    }
    return nullptr;
}

void MainWindow::on_userB_clicked()
{
    if(login){
        int comf=QMessageBox::question(this,"登出","确定退出登录吗？"
                              ,QMessageBox::Ok,QMessageBox::Cancel);
        if(comf!=QMessageBox::Ok){
            readAndShowGoods();
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
        readAndShowGoods();
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
        readAndShowGoods();
        readCart();
        readCards();
        return;
    }
    else{
        u=User();
        c=Cart();
        login=false;
        ui->userLabel->setText("用户未登录");
        ui->userB->setText("登录/注册");
        readAndShowGoods();
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

    //保存选择
    int r=t->currentRow();
    int currentIndex=ui->tabWidget->currentIndex();
    int chooseid=t->item(r,0)->data(Qt::UserRole).toInt();

    readAndShowGoods();//刷新列表，获取最新库存数
    ui->tabWidget->setCurrentIndex(currentIndex);
    t->setCurrentItem(t->item(r,0));
    int id=t->item(r,0)->data(Qt::UserRole).toInt();
    QString name=t->item(r,0)->text();
    int restAmount=t->item(r,4)->data(Qt::UserRole).toInt();

    if(id!=chooseid){//刷新后，表发生变动
        QMessageBox::information(this,"服务当前不可用"
                        ,"抱歉，由于数据库维护，商品发生了变动，请重新选择。");
        return;
    }

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

       /*find<Book>(bookL,id)->addAmount(-amount);//扣库存
        if(saveL())//扣库存保存成功，添加到购物车*/
            c.addProduct(b);
        /*else{
            QMessageBox::information(this,"请求失败！","向库存请求商品失败，请重试");
            find<Book>(bookL,id)->addAmount(amount);
            return;
        }*/
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

        /*find<Elec>(elecL,id)->addAmount(-amount);//扣库存
        if(saveL())//扣库存保存成功，添加到购物车*/
            c.addProduct(e);
        /*else{
            QMessageBox::information(this,"请求失败！","向库存请求商品失败，请重试");
            find<Elec>(elecL,id)->addAmount(amount);
            return;
        }*/
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

        /*find<Clothes>(clothesL,id)->addAmount(-amount);//扣库存
        if(saveL())//扣库存保存成功，添加到购物车*/
            c.addProduct(clo);
        /*else{
            QMessageBox::information(this,"请求失败！","向库存请求商品失败，请重试");
            find<Clothes>(clothesL,id)->addAmount(amount);
            return;
        }*/
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

        /*find<Food>(foodL,id)->addAmount(-amount);//扣库存
        if(saveL())//扣库存保存成功，添加到购物车*/
            c.addProduct(f);
        /*else{
            QMessageBox::information(this,"请求失败！","向库存请求商品失败，请重试");
            find<Food>(foodL,id)->addAmount(amount);
            return;
        }*/
    }
    ui->cartB->setText(QString("我的购物车(%1)").arg(c.getSize()));
    //readAndShowGoods();//刷新库存
    saveCart();
    readCart();
    return;
}

void MainWindow::on_cartB_clicked()
{
    CartDialog dia(&c,&u,this);
    connect(&dia,SIGNAL(delFromCart(int)),this,SLOT(removeFromCart(int)));
    connect(&dia,SIGNAL(pay()),this,SLOT(payCart()));
    dia.setWindowModality(Qt::ApplicationModal);
    dia.exec();
}

void MainWindow::removeFromCart(int index){
    c.delProduct(index);
    ui->cartB->setText(QString("我的购物车(%1)").arg(c.getSize()));
    saveCart();
    readCart();
}

bool MainWindow::saveCart(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"无法保存购物车","无法连接到数据库");
        return false;
    }
    QString tableName=QString("%1_cart").arg(u.getName());
    QSqlQuery q;
    q.prepare(QString("delete from %1").arg(tableName));
    if(!q.exec()){
        QMessageBox::information(this,"保存购物车失败"
                                 ,"保存购物车失败:无法清空表");
        return false;
    }

    vector<Product*> p=c.getListCopy();
    for(auto it=p.begin();it!=p.end();it++){
        Product* &itp=*it;
        if(itp->type()==0)continue;
        int typeCode=itp->type();
        int id=itp->getID();
        id=id*1000+typeCode;
        q.prepare(QString("select amount from %1").arg(tableName));
        q.exec();
        if(q.next()){//重复商品
            int amount=q.value(0).toInt();
            q.prepare(QString("update %1 set amount=? "
                              "where id=?").arg(tableName));
            q.addBindValue(amount+itp->getAmount());
            q.addBindValue(id);
            if(!q.exec()){
                QMessageBox::information(this,"保存购物车"
                    ,QString("一项商品添加失败:%1").arg(itp->getName()));
            }
        }
        else{
            q.prepare(QString("insert into %1(id,amount,name) "
                              "values(?,?,?)").arg(tableName));
            q.addBindValue(id);
            q.addBindValue(itp->getAmount());
            q.addBindValue(itp->getName());
            if(!q.exec()){
                QMessageBox::information(this,"保存购物车"
                    ,QString("一项商品保存失败:%1").arg(itp->getName()));
            }
        }
    }
    return true;
}

void MainWindow::readCart(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        return;
    }
   QString tableName=QString("%1_cart").arg(u.getName());
   QSqlQuery q;
   q.prepare(QString("select id,amount from %1").arg(tableName));
   if(!q.exec()){
       return;
   }
   c.clear();
   while(q.next()){
       int id=q.value(0).toInt();
       int type=id%1000;
       id=id/1000;
       int amount=q.value(1).toInt();

       if(type==0)continue;
       else if(type==1){//Book
           Book* b=find<Book>(bookL,id);
           Book* b_=new Book(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getAuthor());
           c.addProduct(b_);
       }
       else if(type==2){//Elec
           Elec* b=find<Elec>(elecL,id);
           Elec* b_=new Elec(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getBrand());
           c.addProduct(b_);
       }
       else if(type==3){//Clothes
           Clothes* b=find<Clothes>(clothesL,id);
           Clothes* b_=new Clothes(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getSex());
           c.addProduct(b_);
       }
       else if(type==4){//Food
           Food* b=find<Food>(foodL,id);
           Food* b_=new Food(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getDate());
           c.addProduct(b_);
       }
   }
   ui->cartB->setText(QString("我的购物车(%1)").arg(c.getSize()));
}

void MainWindow::readCards(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"获取绑定银行卡失败"
                                 ,"获取绑定银行卡失败：无法连接到数据库，请重试。");
        return;
    }
    QSqlQuery q;
    q.prepare("select bankID, accountID from bankCard where name=?");
    q.addBindValue(u.getName());
    if(!q.exec()){
        QMessageBox::information(this,"获取绑定银行卡失败"
                                 ,"获取绑定银行卡失败：无法查询数据库，请重试。");
        return;
    }
    cards.clear();
    while(q.next()){
        int bankID=q.value(0).toInt();
        QString accountID=q.value(1).toString();
        BankCard bc(bankID,accountID);
        cards.push_back(bc);
    }
}

void MainWindow::on_bankB_clicked()
{
    BankDialog dia(&u,&cards,this);
    if(!dia.initSuccess())
        return;
    connect(&dia,SIGNAL(updateCards()),this,SLOT(readCards()));
    dia.exec();
}

void MainWindow::payCart(){
    readAndShowGoods();
    vector<Product*> p=c.getListCopy();
    for(auto it=p.begin();it!=p.end();it++){//检查库存
        if(dynamic_cast<Book*>(*it)){
            Book* b=dynamic_cast<Book*>(*it);//in cart
            Book* s=find<Book>(bookL,b->getID());//stock
            if(!s||s->getAmount() < b->getAmount()){
                QMessageBox::information(this,"库存不足"
                   ,QString("商品 %1 的库存不足了，请调整购物车")
                                         .arg(b->getName()));
                return;
            }
        }
        else if(dynamic_cast<Elec*>(*it)){
            Elec* b=dynamic_cast<Elec*>(*it);//in cart
            Elec* s=find<Elec>(elecL,b->getID());//stock
            if(!s||s->getAmount() < b->getAmount()){
                QMessageBox::information(this,"库存不足"
                   ,QString("商品 %1 的库存不足了，请调整购物车")
                                         .arg(b->getName()));
                return;
            }
        }
        else if(dynamic_cast<Clothes*>(*it)){
            Clothes* b=dynamic_cast<Clothes*>(*it);//in cart
            Clothes* s=find<Clothes>(clothesL,b->getID());//stock
            if(!s||s->getAmount() < b->getAmount()){
                QMessageBox::information(this,"库存不足"
                   ,QString("商品 %1 的库存不足了，请调整购物车")
                                         .arg(b->getName()));
                return;
            }
        }
        else if(dynamic_cast<Food*>(*it)){
            Food* b=dynamic_cast<Food*>(*it);//in cart
            Food* s=find<Food>(foodL,b->getID());//stock
            if(!s||s->getAmount() < b->getAmount()){
                QMessageBox::information(this,"库存不足"
                   ,QString("商品 %1 的库存不足了，请调整购物车")
                                         .arg(b->getName()));
                return;
            }
        }
    }

    PayDialog dia(c.countPrice(),&cards,this);
    dia.setWindowModality(Qt::ApplicationModal);
    connect(&dia,SIGNAL(payOK()),this,SLOT(payOK()));
    connect(&dia,SIGNAL(payAbort()),this,SLOT(payAbort()));
    dia.exec();
}

void MainWindow::payOK(){
    QMessageBox::information(this,"支付成功"
          ,"去\"已买到的宝贝\"看看吧！");
    bought=c;
    saveBought();
    c.clear();
    bought.clearP();
    readCart();
}

void MainWindow::payAbort(){
    QMessageBox::information(this,"支付操作中止"
          ,"支付操作被用户中止，但是支付请求已经向银行发出。\n"
           "这意味着如果银行卡密码正确，余额充足，银行端非常可能已经完成了这一笔扣款。\n"
           "请务必检查您的银行卡账户，如果对订单有疑问，请联系客服。");
    return;
}

void MainWindow::saveBought(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"无法保存已买到的宝贝","无法连接到数据库");
        return;
    }
    QString tableName=QString("%1_bought").arg(u.getName());
    QSqlQuery q;

    vector<Product*> p=bought.getListCopy();
    for(auto it=p.begin();it!=p.end();it++){
        Product* &itp=*it;
        if(itp->type()==0)continue;
        int typeCode=itp->type();
        int id=itp->getID();
        id=id*1000+typeCode;
        q.prepare(QString("select amount from %1").arg(tableName));
        q.exec();
        if(q.next()){//重复商品
            int amount=q.value(0).toInt();
            q.prepare(QString("update %1 set amount=? "
                              "where id=?").arg(tableName));
            q.addBindValue(amount+itp->getAmount());
            q.addBindValue(id);
            if(!q.exec()){
                QMessageBox::information(this,"保存已买到的宝贝"
                    ,QString("一项商品增加失败:%1").arg(itp->getName()));
            }
        }
        else{
            q.prepare(QString("insert into %1(id,amount,name) "
                              "values(?,?,?)").arg(tableName));
            q.addBindValue(id);
            q.addBindValue(itp->getAmount());
            q.addBindValue(itp->getName());
            if(!q.exec()){
                QMessageBox::information(this,"保存已买到的宝贝"
                    ,QString("一项商品保存失败:%1").arg(itp->getName()));
            }
        }
    }
}

void MainWindow::readBought(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"无法获取已买到的宝贝"
            ,"无法获取已买到的宝贝:无法连接到数据库");
        return;
    }
   QString tableName=QString("%1_bought").arg(u.getName());
   QSqlQuery q;
   q.prepare(QString("select id,amount from %1").arg(tableName));
   if(!q.exec()){
       QMessageBox::information(this,"无法获取已买到的宝贝"
           ,"无法获取已买到的宝贝:无法查询数据库");
       return;
   }
   bought=Cart();
   while(q.next()){
       int id=q.value(0).toInt();
       int type=id%1000;
       id=id/1000;
       int amount=q.value(1).toInt();

       if(type==0)continue;
       else if(type==1){//Book
           Book* b=find<Book>(bookL,id);
           Book* b_=new Book(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getAuthor());
           bought.addProduct(b_);
       }
       else if(type==2){//Elec
           Elec* b=find<Elec>(elecL,id);
           Elec* b_=new Elec(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getBrand());
           bought.addProduct(b_);
       }
       else if(type==3){//Clothes
           Clothes* b=find<Clothes>(clothesL,id);
           Clothes* b_=new Clothes(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getSex());
           bought.addProduct(b_);
       }
       else if(type==4){//Food
           Food* b=find<Food>(foodL,id);
           Food* b_=new Food(b->getID(),b->getName()
                             ,b->getDesc(),b->getPrice()
                             ,amount,b->getDate());
           bought.addProduct(b_);
       }
   }
}

void MainWindow::on_boughtB_clicked()
{
    readBought();
    if(bought.getSize()==0){
        QMessageBox::information(this,
                                 "还没有买到的宝贝",
                                 "还没有买到的宝贝");
        return;
    }
    BoughtDialog dia(&bought);
    dia.exec();
}
