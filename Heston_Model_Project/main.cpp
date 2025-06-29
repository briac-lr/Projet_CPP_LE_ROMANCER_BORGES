#include <Eigen/Core>
#include <iostream>
#include "CallPrice.h"

int main() {
    // Heston model parameters
    const double kappa = 2.0;
    const double theta = 0.04;
    const double sigma = 0.3;
    const double rho = -0.7;
    const double v0 = 0.04;
    const double r = 0.01;

    // Heston Model using constructor with parameters
    HestonModel model(kappa, theta, sigma, rho, v0, r);

    // Heston pricer using constructor with parameters
    HestonPricer pricer(model);

    // Call parameters
    double S0 = 100.0;
    double K = 100.0;
    double T = 1.0;
    double time = 0.0;

    // Compute Heston Call price
    double call_price = pricer.CallHestonPrice(T, time, S0, K);
    std::cout << "Call Heston Price = " << call_price << std::endl;

    // Call price using constructor with parameters
    CallOption call(S0, K, T, r, call_price);

    // Compute implied volatility of the call price
    double impliedVol = call.compute_implied_vol();
    std::cout << "Implied Volatility: " << impliedVol << std::endl;

    return 0;
}
