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
    bool initSuccess(){return initOK;}
private slots:
    void on_addB_clicked();
    void showCards();
    void on_removeB_clicked();

private:
    Ui::BankDialog *ui;
    User* u;
    QVector<BankCard>* cards;
    QMap<int,QString> bankName;
    bool initOK;
signals:
    void updateCards();
};

#endif // BANKDIALOG_H
