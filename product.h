#ifndef PRODUCT_H
#define PRODUCT_H

#include <QtCore>
#include <QtSql>
#include <QMessageBox>

class Product
{
public:
    Product()=default;
    virtual ~Product()=default;
    Product(int id_,QString name_,QString desc_,double price_,int amount_)
        :id(id_),name(name_),desc(desc_),price(price_),amount(amount_){}
    QString getName() const{return name;}
    QString getDesc() const{return desc;}
    double getPrice() const{return price;}
    int getAmount() const{return amount;}
    int getID() const{return id;}
    virtual double countPrice(){return price*amount;}
    virtual bool addAmountAndSave(int add)=0;
    virtual bool save()=0;
    void addAmount(int add){amount+=add;}

private:
    int id;
    QString name;
    QString desc;//description
    double price;
    int amount;

protected:
    bool save(QString tableName);
    double countPrice(QString tableName);
    bool addAmountAndSave(int add,QString tableName);
};

class Book:public Product
{
public:
    Book()=default;
    ~Book()=default;
    Book(int id_,QString name_,QString desc_,double price_,int amount_,QString author_)
        :Product(id_,name_,desc_,price_,amount_),author(author_){}
    double countPrice() override{
        return this->Product::countPrice("Book");
    }
    bool addAmountAndSave(int add) override{
        return this->Product::addAmountAndSave(add,"Book");
    }
    bool save() override{
        return this->Product::save("Book");
    }
    QString getAuthor(){return author;}
private:
    QString author;
};

class Elec:public Product
{
public:
    Elec()=default;
    ~Elec()=default;
    Elec(int id_,QString name_,QString desc_,double price_,int amount_,QString brand_)
        :Product(id_,name_,desc_,price_,amount_),brand(brand_){}
    double countPrice() override{
        return this->Product::countPrice("Elec");
    }
    bool addAmountAndSave(int add) override{
        return this->Product::addAmountAndSave(add,"Elec");
    }
    bool save() override{
        return this->Product::save("Elec");
    }
    QString getBrand(){return brand;}
private:
    QString brand;
};

class Clothes:public Product
{
public:
    enum Sex{General,Male,Female};//0,1,2
    Clothes()=default;
    ~Clothes()=default;
    Clothes(int id_,QString name_,QString desc_,double price_,int amount_,Clothes::Sex sex_)
        :Product(id_,name_,desc_,price_,amount_),sex(sex_){}
    double countPrice() override{
        return this->Product::countPrice("Clothes");
    }
    bool addAmountAndSave(int add) override{
        return this->Product::addAmountAndSave(add,"Clothes");
    }
    bool save() override{
        return this->Product::save("Clothes");
    }
    int getSexi(){return sex==Clothes::Male?1:(sex==Clothes::Female?2:0);}
    Sex getSex(){return sex;}
private:
    Sex sex;
};

class Food:public Product
{
public:
    Food()=default;
    ~Food()=default;
    Food(int id_,QString name_,QString desc_,double price_,int amount_,QDate date_)
        :Product(id_,name_,desc_,price_,amount_),date(date_){}
    double countPrice() override{
        return this->Product::countPrice("Food");
    }
    bool addAmountAndSave(int add) override{
        return this->Product::addAmountAndSave(add,"Food");
    }
    bool save() override{
        return this->Product::save("Food");
    }
    QDate getDate(){return date;}
private:
    QDate date;
};

#endif // PRODUCT_H
