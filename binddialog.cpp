#include "binddialog.h"
#include "ui_binddialog.h"
#include "hash.h"

BindDialog::BindDialog(QString* aid
                       ,QString* pw_
                       ,int* bid
                       ,const QMap<int,QString>& bn_
                       ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BindDialog),
    accountID(aid),
    pw(pw_),
    bankID(bid),
    bankName(bn_)
{
    ui->setupUi(this);
    QMapIterator<int,QString> it(bankName);
    while(it.hasNext()){
        it.next();
        ui->bankSelect->addItem(it.value(),it.key());
    }
}

BindDialog::~BindDialog()
{
    delete ui;
}

void BindDialog::on_applyB_clicked()
{
    *bankID=ui->bankSelect->currentData().toInt();
    *accountID=ui->accountIDIn->text();
    *pw=hash(ui->pwIn->text());
    this->accept();
}

void BindDialog::on_cancelB_clicked()
{
    this->reject();
}
