#include "bankdialog.h"
#include "ui_bankdialog.h"

BankDialog::BankDialog(User* u_
                       ,QVector<BankCard>* c_
                       ,QWidget *parent) :
    QDialog(parent),u(u_),cards(c_),
    ui(new Ui::BankDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"银行"<<"银行卡账号");
    for(auto it=cards->begin();it!=cards->end();it++){

    }
}

BankDialog::~BankDialog()
{
    delete ui;
}
