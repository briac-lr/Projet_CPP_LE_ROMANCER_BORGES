#include <Eigen/Core>
#include <iostream>
#include "CallPrice.h"

int main() {
    // Création du modèle

    const double kappa = 2.0;
    const double theta = 0.04;
    const double sigma = 0.3;
    const double rho = -0.7;
    const double v0 = 0.04;
    const double r = 0.01;

    HestonModel model(kappa, theta, sigma, rho, v0, r);

    // Création du pricer
    HestonPricer pricer(model);

    // Paramètres du call
    double S0 = 100.0;
    double K = 100.0;
    double T = 1.0;
    double time = 0.0;

    // Calcul du prix du call Heston
    double call_price = pricer.CallHestonPrice(T, time, S0, K);
    std::cout << "Call Heston Price = " << call_price << std::endl;

    return 0;
}
