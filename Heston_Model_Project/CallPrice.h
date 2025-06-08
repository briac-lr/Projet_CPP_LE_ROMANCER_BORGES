#ifndef CALLPRICE_H
#define CALLPRICE_H

#include "HestonModel.h"

class HestonPricer {
public:
    HestonModel model;

    HestonPricer(HestonModel model);

    double calculatePrice(double S0, double K, double T);
};


#endif
