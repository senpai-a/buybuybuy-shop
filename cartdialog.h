#ifndef CARTDIALOG_H
#define CARTDIALOG_H

#include <QDialog>
#include "user.h"
#include "cart.h"
#include <QtCore>
#include <QtSql>
#include <QMessageBox>
#include <vector>

namespace Ui {
class CartDialog;
}

class CartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CartDialog(Cart*,User*
                        ,QWidget *parent = 0);
    ~CartDialog();

private:
    Ui::CartDialog *ui;
    Cart *c;
    User *u;

signals:
    void delFromCart(int);
private slots:
    void on_delB_clicked();
    void showCart();
    void showCount();
    void refresh();
    void on_cancelB_clicked();
};

#endif // CARTDIALOG_H
