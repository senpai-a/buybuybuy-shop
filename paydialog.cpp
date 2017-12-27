#include "paydialog.h"
#include "ui_paydialog.h"
#include "serveraddress.h"
#include <QMessageBox>
#include <QInputDialog>
#include "hash.h"
PayDialog::PayDialog(double m
                     ,QVector<BankCard> *c
                     ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayDialog),
    money(m),cards(c)
{
    ui->setupUi(this);

    initBool=false;
    launchBool=false;
    bankName=banks();
    if(bankName.size()==0){
        initBool=true;
        return;
    }
    initUI();
}

PayDialog::~PayDialog()
{
    delete ui;
}

void PayDialog::initUI(){
    soc.close();
    showCards();
    gif.setFileName(":/new/icons/spin.gif");
    ui->waitGifL->setMovie(&gif);
    gif.start();
    ui->waitL->setText("请稍等，正在连接到银行服务器");
    ui->waitGifL->hide();
    ui->waitL->hide();

    ui->moneyL->setText(QString("共需支付：%1 元").arg(money));
    ui->cancelB->setText("取消");
    ui->payB->setText("支付");
    ui->payB->setEnabled(true);
    disconnect(ui->cancelB,SIGNAL(clicked()),this,SLOT(abort()));
    connect(ui->cancelB,SIGNAL(clicked()),this,SLOT(close()));
}

void PayDialog::showCards(){
    ui->tableWidget->clear();
    ui->tableWidget->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(cards->size());
    ui->tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"银行"<<"银行卡账号");
    ui->tableWidget->setColumnWidth(1,300);
    for(auto it=cards->begin();it!=cards->end();it++){
        int i=it-cards->begin();
        int bID=it->getBankID();
        QString aID=it->getAccountID();
        QString bn=bankName.value(bID);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(bn));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(aID));
        ui->tableWidget->item(i,0)->setData(Qt::UserRole,bID);
    }
}

void PayDialog::on_payB_clicked()
{
    QTableWidgetItem* itm=ui->tableWidget->currentItem();
    if(!itm){
        QMessageBox::information(this,"请选择一张银行卡",
                                 "请选择一张银行卡进行支付");
        return;
    }
    int r=ui->tableWidget->currentRow();
    bid=ui->tableWidget->item(r,0)->data(Qt::UserRole).toInt();
    aid=ui->tableWidget->item(r,1)->text();
    bool ok;
    pw=QInputDialog::getText(this,"请输入密码","请输入银行卡密码：",
                          QLineEdit::Password,QString(),&ok);
    if(!ok)return;
    pw=hash(pw);

    soc.connectToHost(SERVER_ADDRESS,SERVER_PORT);
    connect(&soc,SIGNAL(connected()),this,SLOT(connected()));
    ui->waitGifL->show();
    ui->waitL->show();
    ui->cancelB->setText("中止操作");
    ui->payB->setText("正在支付...");
    ui->payB->setEnabled(false);
    disconnect(ui->cancelB,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->cancelB,SIGNAL(clicked()),this,SLOT(abort()));
}

void PayDialog::connected(){
    ui->waitL->setText("请稍等，操作正在进行");
    QString msg=QString(
        "{\"bankID\":%1,\"accountID\":\"%2\",\"pw\":\"%3\",\"pay\":%4}"
      ).arg(bid).arg(aid).arg(pw).arg(money);
    soc.write(msg.toUtf8());
    launchBool=true;
    ui->waitL->setText("扣款请求已发出，正在等待回应");
    connect(&soc,SIGNAL(readyRead()),this,SLOT(readRet()));
}

void PayDialog::readRet(){
    QString ret=QString::fromUtf8(soc.readAll());
    if(ret=="1"){
        this->close();
        emit(payOK());        
        return;
    }
    else if(ret=="0")
        QMessageBox::information(this,"支付失败",
                                 "银行卡密码错误，请重试");
    else if(ret=="x")
        QMessageBox::information(this,"支付失败",
                                 "网络通信错误，支付失败，请重试");
    else if(ret=="d")
        QMessageBox::information(this,"支付失败",
                                 "银行业务服务暂时不可用，请稍后重试");
    else if(ret=="$")
        QMessageBox::information(this,"支付失败",
                                 "这张卡余额不足，请重试或按\"取消\"返回");
    initUI();
}

void PayDialog::abort(){
    initUI();
    if(!launchBool){
        return;
    }
    this->close();
    emit(payAbort());    
}


