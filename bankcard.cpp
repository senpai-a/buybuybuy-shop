#include "bankcard.h"
#include <QtNetwork>
#include <QMessageBox>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

using namespace rapidjson;

QMap<int,QString> banks(){
    QTcpSocket s;
    QMap<int,QString> ret;
    s.connectToHost(QHostAddress::LocalHost,11111);
    if(s.state()!=QTcpSocket::ConnectedState){
        QMessageBox::information(0,"网络连接错误"
            ,"银行服务器正在维护或无法连接到互联网，请稍后重试");
        ret.clear();
        return ret;
    }
    s.write(QString("banks").toUtf8());
    if(!s.waitForBytesWritten(-1)||!s.waitForReadyRead(-1)){
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
        QString name=QString(d[i]["name"].GetString());
        ret.insert(bankID,name);
    }
    return ret;
}
