#ifndef BANKDIALOG_H
#define BANKDIALOG_H

#include <QDialog>
#include "user.h"
#include "bankcard.h"
namespace Ui {
class BankDialog;
}

class BankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BankDialog(User* u_
                        ,QVector<BankCard>* c_
                        ,QWidget *parent = 0);
    ~BankDialog();

private:
    Ui::BankDialog *ui;
    User* u;
    QVector<BankCard>* cards;
    QMap<int,QString> bankName;
};

QMap<int,QString> banks();

#endif // BANKDIALOG_H
