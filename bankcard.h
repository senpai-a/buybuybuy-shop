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
private:
    int bankID;
    QString accountID;
};

#endif // BANKCARD_H
