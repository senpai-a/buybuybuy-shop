#include "bankcard.h"
#include <QtNetwork>
#include <QMessageBox>
#include "serveraddress.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

using namespace rapidjson;

QMap<int,QString> banks(){
    QTcpSocket s;
    QMap<int,QString> ret;
    s.connectToHost(SERVER_ADDRESS,SERVER_PORT);
    if(!s.waitForConnected(1000)){
        QMessageBox::information(0,"网络连接错误"
            ,"银行服务器正在维护或无法连接到互联网，请稍后重试");
        ret.clear();
        return ret;
    }
    s.write(QString("banks").toUtf8());
    if(!s.waitForBytesWritten(1000)||!s.waitForReadyRead(3000)){
        QMessageBox::information(0,"网络连接错误"
            ,"电波传达不到呢，请稍后重试");
        ret.clear();
        return ret;
    }
    QByteArray msg=s.readAll();
    Document d;
    if(d.Parse(msg.data()).HasParseError()){
        QMessageBox::information(0,"网络连接错误"
            ,"消息损坏，请稍后重试");
        qDebug()<<msg;
        ret.clear();
        return ret;
    }
    if(!d.IsArray()){
        QMessageBox::information(0,"网络连接错误"
            ,"消息不符合协议，请联系管理员");
        qDebug()<<msg;
        ret.clear();
        return ret;
    }
    for(SizeType i=0;i<d.Size();i++){
        int bankID=d[i]["bankID"].GetInt();
        QString name=QString::fromUtf8(d[i]["name"].GetString());
        ret.insert(bankID,name);
    }
    return ret;
}

bool BankCard::checkPw(const QString& pw){
    QTcpSocket s;
    s.connectToHost(SERVER_ADDRESS,SERVER_PORT);
    if(!s.waitForConnected(1000)){
        QMessageBox::information(0,"网络连接错误"
            ,"银行服务器正在维护或无法连接到互联网，请稍后重试");
        return false;
    }
    QString quest;
    quest=QString("{\"bankID\":%1,\"accountID\":\"%2\",\"pw\":\"%3\"}")
            .arg(bankID).arg(accountID).arg(pw);
    s.write(quest.toUtf8());
    if(!s.waitForBytesWritten(1000)||!s.waitForReadyRead(3000)){
        QMessageBox::information(0,"网络连接错误"
            ,"电波传达不到呢，请稍后重试");
        return false;
    }
    QByteArray msg=s.readAll();
    if(msg=="0"){
        QMessageBox::information(0,"银行卡密码错误"
            ,"银行卡密码错误，请重试");
        return false;
    }
    else if(msg=="1")return true;
    else if(msg=="x"){
        QMessageBox::information(0,"网络错误"
            ,"银行服务报告了一个网络错误，请重试");
        return false;
    }
    else if(msg=="d"){
        QMessageBox::information(0,"操作失败"
            ,"银行服务目前暂时不可用，请稍后重试");
        return false;
    }

    QMessageBox::information(0,"银行服务错误"
        ,"银行服务错误：返回消息不符合协议，请联系管理员。");
    return false;
}
