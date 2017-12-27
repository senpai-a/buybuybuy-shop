#ifndef BOUGHTDIALOG_H
#define BOUGHTDIALOG_H

#include <QDialog>
#include "cart.h"

namespace Ui {
class BoughtDialog;
}

class BoughtDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoughtDialog(Cart* bought,QWidget *parent = 0);
    ~BoughtDialog();

private:
    Ui::BoughtDialog *ui;
    Cart* bought;
    void showCart();
};

#endif // BOUGHTDIALOG_H
