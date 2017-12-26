#include "bankdialog.h"
#include "ui_bankdialog.h"

BankDialog::BankDialog(User* u_
                       ,QVector<BankCard>* c_
                       ,QWidget *parent) :
    QDialog(parent),u(u_),cards(c_),
    ui(new Ui::BankDialog)
{
    ui->setupUi(this);

    bankName=banks();
    if(bankName.size()==0){
        this->reject();
    }

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(cards->size());
    ui->tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"银行"<<"银行卡账号");

    for(auto it=cards->begin();it!=cards->end();it++){
        int i=it-cards->begin();
        int bID=it->getBankID();
        QString aID=it->getAccountID();
        QString bn=bankName.value(bID);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(bn));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(aID));
        ui->tableWidget->item(i,0)->setData(Qt::UserRole,bID);
    }
}

BankDialog::~BankDialog()
{
    delete ui;
}
