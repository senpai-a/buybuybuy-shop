#ifndef CART_H
#define CART_H
#include "product.h"
#include <vector>
using namespace std;

class Cart
{
public:
    Cart()=default;
    double countPrice();
    void addProduct(Product *p){products.push_back(p);}
    void delProduct(int index){products.erase(products.begin()+index);}
private:
    vector<Product*> products;
};

#endif // CART_H
