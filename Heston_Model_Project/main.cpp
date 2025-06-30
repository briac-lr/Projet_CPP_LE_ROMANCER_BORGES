#include <Eigen/Core>
#include <iostream>
#include <array>

#include "CallPrice.h"
#include "CSVReader.h"
#include "Calibration.h"


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
    const double S0 = 498.63;
    const double K = 500.0;
    const double T = 1.0;
    const double time = 0.0;

    // Compute Heston Call price
    double call_price = pricer.CallHestonPrice(T, time, S0, K);
    std::cout << "Call Heston Price = " << call_price << std::endl;

    // Call price using constructor with parameters
    CallOption call(S0, K, T, r, call_price);

    // Compute implied volatility of the call price
    double impliedVol = call.compute_implied_vol();
    std::cout << "Implied Volatility: " << impliedVol << std::endl;

    const std::string market_data_path = "C:/Users/briac/OneDrive/ENPC3A/semestre1/C++/Projet_CPP_LE_ROMANCER_BORGES/Heston_Model_Project/volatility_surface.csv";

    // initial_guess = {rho, kappa, theta, v0, sigma}
    std::array<double, 5> initial_guess = { -0.5, 2.0, 0.05, 0.05, 0.30 };
    const int max_iterations = 500;

    Calibration calibrator(initial_guess, max_iterations, market_data_path);
    calibrator.Calibrate();

    std::cout << "\n=== Calibrated Heston parameters ===\n"
        << "rho   = " << calibrator.getRho() << '\n'
        << "kappa = " << calibrator.getKappa() << '\n'
        << "theta = " << calibrator.getTheta() << '\n'
        << "v0    = " << calibrator.getV0() << '\n'
        << "sigma = " << calibrator.getSigma() << "\n\n";


    return 0;
}