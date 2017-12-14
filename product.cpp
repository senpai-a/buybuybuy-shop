#include "product.h"

double Product::countPrice(QString tableName){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"错误","计价时发生错误：无法连接到商品数据库，请重试");
        return -1;
    }
    QSqlQuery q;
    q.prepare("select cutLine,cutOff,factor from sale "
              "where tableName = ?");
    q.addBindValue(tableName);
    if(!q.exec()){
        QMessageBox::information(0,"错误","计价时发生错误：无法查询商品数据库，请重试");
        return -1;
    }
    if(!q.next()){
        QMessageBox::information(0,"警告"
             ,QString("计价：商品数据库无%1品类折扣信息，请联系管理员")
              .arg(tableName));
        return amount*price;
    }
    double cutLine=q.value(0).toDouble();
    double cutOff=q.value(1).toDouble();
    double factor=q.value(2).toDouble();
    double ret=amount*price;
    if(cutLine>0&&cutOff>0&&ret>cutLine)
        ret-=cutOff;
    if(factor>=0&&factor<=1)
        ret*=factor;
    return ret;
}

bool Product::addAmountAndSave(int add,QString tableName){
    amount+=add;
    if(save(tableName))return true;
    amount-=add;
    return false;
}

bool Product::save(QString tableName){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"失败","向数据库提交购买请求失败:无法连接到数据库，请重试");
    }
    QSqlQuery q;
    q.prepare("update ? set amount = ? "
              "where ID = ?");
    q.addBindValue(tableName);
    q.addBindValue(amount);
    q.addBindValue(id);
    return q.exec();
}
