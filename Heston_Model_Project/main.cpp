#include "CallPrice.h"
#include "CSVReader.h"
#include "Calibration.h"

#include <iostream>
#include <array>
#include <fstream>

#include <Eigen/Core>


int main() {
    // Heston model parameters
    const double kappa = 2.0;
    const double theta = 0.04;
    const double sigma = 0.5;
    const double rho = -0.4;
    const double v0 = 0.03;

    // Call parameters
    const double S0 = 212.44; // Spot price at the close of the Apple stock 02/07/2025
    const double time = 0.0;
    const double r = 0.0423; // US tresory bond rate

    // Data file path
    const std::string market_data_path = "volatility_surface.csv";

    // Initial values for rho, kappa, theta, v0, sigma
    std::array<double, 5> initial_guess = {rho, kappa, theta, v0, sigma};

    // Number of iterations
    const int max_iterations = 100;

    // Calibrate
    Calibration calibrator(initial_guess, max_iterations, market_data_path, r, S0, time);
    calibrator.Calibrate();

    std::cout << "\n=== Calibrated Heston parameters ===\n"
        << "rho   = " << calibrator.getRho() << '\n'
        << "kappa = " << calibrator.getKappa() << '\n'
        << "theta = " << calibrator.getTheta() << '\n'
        << "v0    = " << calibrator.getV0() << '\n'
        << "sigma = " << calibrator.getSigma() << "\n\n";


    // Get the calibrated parameters
    double calibrated_rho = calibrator.getRho();
    double calibrated_kappa = calibrator.getKappa();
    double calibrated_theta = calibrator.getTheta();
    double calibrated_v0 = calibrator.getV0();
    double calibrated_sigma = calibrator.getSigma();

    // Reprice with calibrated parameters
    HestonModel calibrated_model(calibrated_kappa, calibrated_theta, calibrated_sigma, calibrated_rho, calibrated_v0, r);
    HestonPricer calibrated_pricer(calibrated_model);

    // CSV reader
    CSVReader reader(market_data_path);
    auto market_data = reader.read();

    // Open CSV output file
    std::ofstream output("calibrated_output.csv");
    if (!output.is_open()) {
        std::cerr << "Error : unable to open output file." << std::endl;
        return 1;
    }

    // Write headers
    output << "CallPrice,Implied Volatility,Strike,Maturity\n";

    for (const auto& row : market_data) {
        double market_vol = row[1];
        double strike = row[2];
        double maturity = row[3];

        // Compute call price with calibrated Heston model
        double call_price = calibrated_pricer.CallHestonPrice(maturity, time, S0, strike);

        CallOption option(S0, strike, maturity, r, call_price);

        try {
            // Compute implied volatility
            double implied_vol = option.compute_implied_vol();
            output << call_price << "," << implied_vol << "," << strike << "," << maturity << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error on strike=" << strike << ", maturity=" << maturity
                << " : " << e.what() << "\n";
        }
    }

    output.close();
    std::cout << "CSV file 'calibrated_output.csv' successfully generated.\n";

    return 0;
}