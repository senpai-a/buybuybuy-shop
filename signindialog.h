#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>
#include "user.h"

namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(User* u,QWidget *parent = 0);
    ~SignInDialog();

private slots:
    void on_loginB_clicked();

    void on_signupB_clicked();

private:
    Ui::SignInDialog *ui;
    User* u;
};

#endif // SIGNINDIALOG_H
