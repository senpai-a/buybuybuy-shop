#include "user.h"

bool User::checkPw(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"登陆失败","无法连接到数据库，请重试");
        return false;
    }
    QSqlQuery q;
    q.prepare("select name from User where "
              "name=? and pw=?");
    q.addBindValue(name);
    q.addBindValue(pw);
    if(!q.exec()){
        QMessageBox::information(0,"登陆失败","无法查询数据库，请重试");
        return false;
    }
    if(!q.next()){
        QMessageBox::information(0,"登陆失败","用户名或密码错误，请重试");
        return false;
    }
    return true;
}

bool User::signUp(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"注册失败","无法连接到数据库，请重试");
        return false;
    }
    QSqlQuery q;
    q.prepare("select name from User where "
              "name=?");
    q.addBindValue(name);
    if(!q.exec()){
        QMessageBox::information(0,"注册失败","无法查询数据库，请重试");
        return false;
    }
    if(q.next()){
        QMessageBox::information(0,"注册失败","用户名已存在，请重试");
        return false;
    }
    QSqlQuery q1;
    /*QString queryStr=QString("insert into User (name,pw) "
                           "values(?,?);"
                           "create table %1_cart( "
                           "id integer primary key, "
                           "amount integer, "
                           "name text);"
                           "create table %1_bought( "
                           "id integer primary key, "
                           "amount integer, "
                           "name text);").arg(name);
    q1.prepare(queryStr);
    qDebug()<<queryStr;*/
    q1.prepare("insert into User (name,pw) "
               "values(?,?)");
    q1.addBindValue(name);
    q1.addBindValue(pw);
    if(!q1.exec()){
        QMessageBox::information(0,"注册失败","注册失败");
        return false;
    }
    q1.prepare(QString("create table %1_cart( "
               "id integer primary key, "
               "amount integer, "
               "name text);").arg(name));
    if(!q1.exec()){
        QMessageBox::information(0,"注册失败","建立购物车表失败");
        return false;
    }
    q1.prepare(QString("create table %1_bought( "
               "id integer primary key, "
               "amount integer, "
               "name text);").arg(name));
    if(!q1.exec()){
        QMessageBox::information(0,"注册失败","建立已买到的宝贝表失败");
        return false;
    }
    return true;
}
