#include "cart.h"

double Cart::countPrice(){
    double ret=0;
    for(auto it=products.begin();it!=products.end();it++){
        ret+=(*it)->countPrice();
    }
    return ret;
}
