#include <Eigen/Core>
#include <iostream>
#include "CallPrice.h"
#include "CSVReader.h"

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


    try {
        csv::Reader reader("C:/Users/briac/OneDrive/ENPC3A/semestre1/C++/Projet_CPP_LE_ROMANCER_BORGES/Heston_Model_Project/volatility_surface.csv"); // CSV avec en-tête et virgules
        std::vector<std::string> row;

        // Affiche l’en-tête
        for (const auto& colName : reader.header())
            std::cout << colName << '\t';
        std::cout << '\n' << std::string(40, '-') << '\n';

        // Parcours ligne par ligne
        while (reader.readRow(row)) {
            for (const auto& cell : row)
                std::cout << cell << '\t';
            std::cout << '\n';
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << '\n';
        return 1;
    }
    return 0;
}