#ifndef BINDDIALOG_H
#define BINDDIALOG_H

#include <QDialog>
#include <QMap>
namespace Ui {
class BindDialog;
}

class BindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BindDialog(QString* accountID
                        ,QString* pw
                        ,int* bankID
                        ,const QMap<int,QString> &bankName_
                        ,QWidget *parent = 0);
    ~BindDialog();

private slots:
   void on_applyB_clicked();

   void on_cancelB_clicked();

private:
    Ui::BindDialog *ui;
    QString *accountID;
    QString *pw;
    int *bankID;
    QMap<int,QString> bankName;
};

#endif // BINDDIALOG_H
