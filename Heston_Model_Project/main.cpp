#include <Eigen/Core>
#include <iostream>
#include "CallPrice.h"

int main() {
    // Création du modèle
    HestonModel model;
    model._kappa = 2.0;
    model._theta = 0.04;
    model._sigma = 0.3;
    model._rho = -0.7;
    model._v0 = 0.04;
    model._r = 0.01;

    // Création du pricer
    HestonPricer pricer(model);

    // Paramètres du call
    double S0 = 100.0;
    double K = 100.0;
    double T = 1.0;
    double time = 0.0;

    // Tester RealIntegrand à un point
    double omega = 0.5;
    double integrand_value = pricer.RealIntegrand(omega, T, time, S0, K, true);
    std::cout << "RealIntegrand(" << omega << ") = " << integrand_value << std::endl;

    // Tester ComputeP (l'intégrale sur [-1, 1])
    double P1 = pricer.ComputeP(T, time, S0, K, true);
    double P2 = pricer.ComputeP(T, time, S0, K, false);
    std::cout << "P1 = " << P1 << ", P2 = " << P2 << std::endl;

    // Calcul du prix du call Heston
    double call_price = S0 * P1 - K * std::exp(-model._r * T) * P2;
    std::cout << "Call Heston Price = " << call_price << std::endl;

    return 0;
}
