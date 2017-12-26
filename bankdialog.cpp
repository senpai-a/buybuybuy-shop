#include "bankdialog.h"
#include "ui_bankdialog.h"
#include "binddialog.h"
BankDialog::BankDialog(User* u_
                       ,QVector<BankCard>* c_
                       ,QWidget *parent) :
    QDialog(parent),ui(new Ui::BankDialog)
    ,u(u_),cards(c_),initOK(true)
{
    ui->setupUi(this);

    bankName=banks();
    if(bankName.size()==0){
        initOK=false;
        return;
    }

    showCards();
}

BankDialog::~BankDialog()
{
    delete ui;
}

void BankDialog::showCards(){
    ui->tableWidget->clear();
    ui->tableWidget->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(cards->size());
    ui->tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"银行"<<"银行卡账号");
    ui->tableWidget->setColumnWidth(1,300);
    for(auto it=cards->begin();it!=cards->end();it++){
        int i=it-cards->begin();
        int bID=it->getBankID();
        QString aID=it->getAccountID();
        QString bn=bankName.value(bID);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(bn));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(aID));
        ui->tableWidget->item(i,0)->setData(Qt::UserRole,bID);
    }
}

void BankDialog::on_addB_clicked()
{
    int bankID;
    QString accountID;
    QString pw;
    BindDialog dia(&accountID,&pw,&bankID,bankName,this);
    dia.exec();
    if(dia.result()!=QDialog::Accepted){
        return;
    }
    BankCard bankCard(bankID,accountID);
    if(!bankCard.checkPw(pw)){
        //QMessageBox::information(this,"密码错误","银行卡密码错误，请重试");
        return;
    }
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"绑定失败"
                                 ,"无法连接到数据库，请重试");
        return;
    }
    QSqlQuery q;
    q.prepare("select ID from bankCard "
              "where name=? "
              "and accountID=? "
              "and bankID=?");//查重
    q.addBindValue(u->getName());
    q.addBindValue(accountID);
    q.addBindValue(bankID);
    if(!q.exec()){
        QMessageBox::information(this,"绑定失败"
                                 ,"无法查询数据库:0，请重试");
        return;
    }
    if(q.next()){
        QMessageBox::information(this,"绑定重复"
                                 ,"您已经绑定过这张银行卡");
        return;
    }

    q.prepare("select nextID from nextID "
              "where tableName=\"bankCard\"");
    if(!q.exec()){
        QMessageBox::information(this,"绑定失败"
                                 ,"无法查询数据库:1，请重试");
        return;
    }
    if(!q.next()){
        QMessageBox::information(this,"绑定失败"
                                 ,"数据库损坏，请联系管理员");
        return;
    }
    int id=q.value(0).toInt();
    q.prepare("update nextID set nextID=? "
              "where tableName=\"bankCard\"");
    q.addBindValue(id+1);
    if(!q.exec()){
        QMessageBox::information(this,"绑定失败"
                                 ,"无法更新数据库:0，请重试");
        return;
    }
    q.prepare("insert into bankCard(ID,name,accountID,bankID) "
              "values(?,?,?,?)");
    q.addBindValue(id);
    q.addBindValue(u->getName());
    q.addBindValue(accountID);
    q.addBindValue(bankID);
    if(!q.exec()){
        QMessageBox::information(this,"绑定失败"
                                 ,"无法更新数据库:1");
        q.prepare("update nextID set nextID=? "
                  "where tableName=\"bankCard\"");
        q.addBindValue(id);
        q.exec();//尝试撤销nextID++,失败不造成破坏：ID跳过1
        return;
    }
    QMessageBox::information(this,"绑定银行卡"
                             ,"绑定成功");
    emit(updateCards());
    showCards();
}

void BankDialog::on_removeB_clicked()
{
    QTableWidgetItem* itm=ui->tableWidget->currentItem();
    if(!itm){
        QMessageBox::information(this,"选择一个项"
                                 ,"选择一个你要解除绑定的银行卡");
        return;
    }
    int r=ui->tableWidget->currentRow();
    QString name=u->getName();
    QString aid=ui->tableWidget->item(r,1)->text();
    int bid=ui->tableWidget->item(r,0)->data(Qt::UserRole).toInt();
    int confirm=QMessageBox::question(this,"确定解除绑定吗"
        ,QString("你将解除绑定:%1 账号:%2").arg(bankName[bid]).arg(aid)
        ,QMessageBox::Cancel,QMessageBox::Ok);
    if(confirm!=QMessageBox::Ok){
        return;
    }
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"操作失败"
                                 ,"无法连接到数据库，请重试");
        return;
    }
    QSqlQuery q;
    q.prepare("delete from bankCard "
              "where name=? "
              "and bankID=? "
              "and accountID=?");
    q.addBindValue(name);
    q.addBindValue(bid);
    q.addBindValue(aid);

    if(!q.exec()){
        QMessageBox::information(this,"操作失败"
                                 ,"无法从数据库删除，请重试");
        return;
    }

    emit(updateCards());
    showCards();
}
