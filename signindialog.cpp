#include "signindialog.h"
#include "ui_signindialog.h"

SignInDialog::SignInDialog(User* u_,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignInDialog)
{
    ui->setupUi(this);
    u=u_;
}

SignInDialog::~SignInDialog()
{
    delete ui;
}

void SignInDialog::on_loginB_clicked()
{
    QString name=ui->nameIn->text();
    QString pw=ui->pwIn->text();

    *u=User(name,pw);
    if(u->checkPw()){
        this->accept();
    }
    else{
        this->reject();
    }
}

void SignInDialog::on_signupB_clicked()
{
    QString name=ui->nameIn->text();
    QString pw=ui->pwIn->text();
    if(name.size()<3){
        QMessageBox::information(this,"用户名太短","请输入一个长度至少为3的用户名");
        return;
    }
    if(pw.size()<6){
        QMessageBox::information(this,"密码太短","请输入一个长度至少为6的密码");
        return;
    }
    *u=User(name,pw);
    if(u->signUp()){
        this->accept();
    }
    else{
        this->reject();
    }
}
