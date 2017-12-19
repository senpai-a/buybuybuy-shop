#ifndef BANKDIALOG_H
#define BANKDIALOG_H

#include <QDialog>

namespace Ui {
class BankDialog;
}

class BankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BankDialog(QWidget *parent = 0);
    ~BankDialog();

private:
    Ui::BankDialog *ui;
};

#endif // BANKDIALOG_H
