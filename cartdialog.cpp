#include "cartdialog.h"
#include "ui_cartdialog.h"

CartDialog::CartDialog(Cart* c_,User* u_
                       ,QWidget *parent):
    QDialog(parent),
    ui(new Ui::CartDialog)
{
    c=c_;
    u=u_;
    ui->setupUi(this);
    refresh();
}

CartDialog::~CartDialog()
{
    delete ui;
}

void CartDialog::on_delB_clicked()
{
    int r=ui->tableWidget->currentRow();
    if(!ui->tableWidget->currentItem()){
        QMessageBox::information(this,"从购物车移除","请选择一项");
        return;
    }
    int comf=QMessageBox::question(this,"确认从购物车移除商品"
                                   ,QString("确认移除：%1")
                                   .arg(ui->tableWidget->item(r,0)->text())
                                   ,QMessageBox::Ok,QMessageBox::Cancel);
    if(comf!=QMessageBox::Ok)return;
    emit delFromCart(r);
    refresh();
}

void CartDialog::showCart(){
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(c->getSize());
    ui->tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"品名"<<"描述"<<"单价"<<"数量");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    vector<Product*> products=c->getListCopy();
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

void CartDialog::showCount(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"计价失败","无法连接数据库,请重试");
        return;
    }
    ui->saleL->setText(QString());
    QSqlQuery q;
    q.prepare("select tableName,cutLine,cutOff,factor from sale");
    if(q.exec()){
       while(q.next()){
           QString tableName=q.value(0).toString();
           if(tableName=="Book")tableName="书籍";
           if(tableName=="Elec")tableName="电子";
           if(tableName=="Clothes")tableName="服装";
           if(tableName=="Food")tableName="食品";
           QString line;
           double cutLine=q.value(1).toDouble();
           double cutOff=q.value(2).toDouble();
           double factor=q.value(3).toDouble();
           if((cutLine>0&&cutOff>0)||(factor>=0&&factor<=1)){
                line=QString("%1品类 %2 %3\n")
                        .arg(tableName)
                        .arg(cutLine>0&&cutOff>0?
                                 QString("满%1元减%2元").arg(cutLine).arg(cutOff)
                               :QString())
                        .arg(factor>=0&&factor<=1?
                                 QString("打%1折").arg(factor*10)
                                :QString());
           }
           ui->saleL->setText(
                            ui->saleL->text()+line
                       );
       }
    }
    ui->countL->setText(QString("共需支付：%1 元").arg(c->countPrice()));
}

void CartDialog::refresh(){
    showCart();
    showCount();
}

void CartDialog::on_cancelB_clicked()
{
    this->reject();
}
