#ifndef CART_H
#define CART_H
#include "product.h"
#include <vector>
using namespace std;

class Cart
{
public:
    Cart()=default;
    ~Cart();
    int getSize(){return products.size();}
    double countPrice();
    void addProduct(Product *p){
        products.push_back(p);
    }
    void delProduct(int index){
        delete products[index];
        products.erase(products.begin()+index);
    }
    void clear(){
        for(auto it=products.begin();it!=products.end();it++){
            qDebug()<<it-products.begin()<<(*it)->getName();
            delete *it;
        }
        products.clear();
    }
    void clearP(){
        products.clear();
    }

    vector<Product*> getListCopy(){return products;}
private:
    vector<Product*> products;
};

#endif // CART_H
