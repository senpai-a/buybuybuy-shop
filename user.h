#ifndef USER_H
#define USER_H
#include <QtCore>
#include <QMessageBox>
#include <QtSql>
#include "hash.h"

class User
{
public:
    User()=default;
    User(QString name_,QString pw_):name(name_),pw(hash(pw_)){}
    QString getName(){return name;}
    bool checkPw();//check current name and pw from DB
    bool signUp();//sign up with current name and pw
private:
    QString name;
    QString pw;
};

#endif // USER_H
