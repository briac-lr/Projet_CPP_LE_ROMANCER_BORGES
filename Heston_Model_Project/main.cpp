#include <Eigen/Core>
#include <iostream>
#include <array>
#include <fstream>

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
    const double r = 0.0423;

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
    std::array<double, 5> initial_guess = { -0.8, 4.0, 0.04, 0.04, 0.50 };
    const int max_iterations = 100;

    Calibration calibrator(initial_guess, max_iterations, market_data_path);
    calibrator.Calibrate();

    std::cout << "\n=== Calibrated Heston parameters ===\n"
        << "rho   = " << calibrator.getRho() << '\n'
        << "kappa = " << calibrator.getKappa() << '\n'
        << "theta = " << calibrator.getTheta() << '\n'
        << "v0    = " << calibrator.getV0() << '\n'
        << "sigma = " << calibrator.getSigma() << "\n\n";


    // Récupération des paramètres calibrés
    double calibrated_rho = calibrator.getRho();
    double calibrated_kappa = calibrator.getKappa();
    double calibrated_theta = calibrator.getTheta();
    double calibrated_v0 = calibrator.getV0();
    double calibrated_sigma = calibrator.getSigma();

    // Recréation du modèle Heston avec les paramètres calibrés
    HestonModel calibrated_model(calibrated_kappa, calibrated_theta, calibrated_sigma,
        calibrated_rho, calibrated_v0, r);
    HestonPricer calibrated_pricer(calibrated_model);

    // Lecture des données du marché
    CSVReader reader(market_data_path);
    auto market_data = reader.read();

    // Ouverture du fichier CSV de sortie
    std::ofstream output("calibrated_output.csv");
    if (!output.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier de sortie." << std::endl;
        return 1;
    }

    // Écriture de l'en-tête
    output << "CallPrice,Implied Volatility,Strike,Maturity\n";

    // Boucle sur chaque option du marché
    for (const auto& row : market_data) {
        double market_vol = row[1];
        double strike = row[2];
        double maturity = row[3];

        // Calcul du prix du call selon Heston calibré
        double call_price = calibrated_pricer.CallHestonPrice(maturity, 0.0, S0, strike);

        // Calcul de la volatilité implicite
        CallOption option(S0, strike, maturity, r, call_price);

        try {
            double implied_vol = option.compute_implied_vol();
            output << call_price << "," << implied_vol << "," << strike << "," << maturity << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur sur strike=" << strike << ", maturity=" << maturity
                << " : " << e.what() << "\n";
        }
    }

    output.close();
    std::cout << "Fichier CSV 'calibrated_output.csv' généré avec succès.\n";
    

    return 0;
}