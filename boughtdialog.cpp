#include "boughtdialog.h"
#include "ui_boughtdialog.h"

BoughtDialog::BoughtDialog(Cart* b,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoughtDialog),
    bought(b)
{
    ui->setupUi(this);
    showCart();
}

void BoughtDialog::showCart(){
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(bought->getSize());
    ui->tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"品名"<<"描述"<<"单价"<<"数量");
    vector<Product*> products=bought->getListCopy();
    for(auto it=products.begin();it!=products.end();it++){
        int i=it-products.begin();
        Product &p=**it;
        QTableWidget &t=*(ui->tableWidget);
        t.setItem(i,0,new QTableWidgetItem(p.getName()));
        t.setItem(i,1,new QTableWidgetItem(p.getDesc()));
        t.setItem(i,2,new QTableWidgetItem(QString("%1 元").arg(p.getPrice())));
        t.setItem(i,3,new QTableWidgetItem(QString("%1").arg(p.getAmount())));
        t.item(i,0)->setData(Qt::UserRole,p.getID());
        t.item(i,2)->setData(Qt::UserRole,p.getPrice());
        t.item(i,3)->setData(Qt::UserRole,p.getAmount());
    }
}

BoughtDialog::~BoughtDialog()
{
    delete ui;
}
