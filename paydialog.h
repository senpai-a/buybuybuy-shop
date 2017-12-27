#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QMovie>
#include <QDialog>
#include "bankcard.h"
#include <QtNetwork>

namespace Ui {
class PayDialog;
}

class PayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PayDialog(double money
                       ,QVector<BankCard> *cards
                       ,QWidget *parent = 0);
    ~PayDialog();
    bool initOK(){return initBool;}
private:
    Ui::PayDialog *ui;
    QMovie gif;

    double money;
    int bid;
    QString aid;
    QString pw;
    QVector<BankCard> *cards;
    QMap<int,QString> bankName;
    bool initBool;
    QTcpSocket soc;
    bool launchBool;//付款请求是否已经发射

private slots:
   void initUI();//关闭soc，重置ui
   void showCards();
   void on_payB_clicked();
   void connected();
   void abort();
   void readRet();

signals:
   void payOK();
   void payAbort();
};

#endif // PAYDIALOG_H
