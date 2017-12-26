#ifndef BANKCARD_H
#define BANKCARD_H
#include <QtCore>
#include <QtSql>

class BankCard
{
public:
    BankCard()=default;
    BankCard(int bankID_,QString accountID_)
        :bankID(bankID_),accountID(accountID_){}
    bool checkPw(const QString& pw);
    int getBankID(){return bankID;}
    QString getAccountID(){return accountID;}
private:
    int bankID;
    QString accountID;
};

QMap<int,QString> banks();//从服务器获取银行id对应名字

#endif // BANKCARD_H
