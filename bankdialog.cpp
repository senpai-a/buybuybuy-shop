#include "bankdialog.h"
#include "ui_bankdialog.h"

BankDialog::BankDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BankDialog)
{
    ui->setupUi(this);
}

BankDialog::~BankDialog()
{
    delete ui;
}
