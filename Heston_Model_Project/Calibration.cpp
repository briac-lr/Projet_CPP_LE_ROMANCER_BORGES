#include "Calibration.h"

#include <cstring>
#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>



// Parameterized constructor   
Calibration::Calibration(const std::array<double, 5>& initial_guess, int iterations, const char* market_data_path)
    : _initial_guess(initial_guess), _number_iterations(iterations), _market_data_path(market_data_path),
    _rho(0), _kappa(0), _theta(0), _v0(0), _sigma(0)
{

}

// Copy constructor
Calibration::Calibration(const Calibration& other)
    :_initial_guess(initial_guess),
    _number_iterations(other._number_iterations), 
    _market_data_path(other._market_data_path),
    _rho(other._rho), _kappa(other._kappa), _theta(other._theta), _v0(other._v0), _sigma(other._sigma)
{

}


// Copy assignment operator
Calibration& Calibration::operator=(const Calibration& other) {
    if (this != &other) {

        _number_iterations = other._number_iterations;
        _market_data_path = other._market_data_path;
        _initial_guess = other._initial_guess;

        _rho = other._rho;
        _kappa = other._kappa;
        _theta = other._theta;
        _v0 = other._v0;
        _sigma = other._sigma;
    }
    return *this;
}

// Destructor
Calibration::~Calibration() {
    delete[] _initial_guess;
}

// Calibration method (dummy logic for now)
void Calibration::Calibrate() {
    std::vector<std::vector<double>> data = get_data(_market_data_path);

    // Use the initial guess as "calibrated" values
    _rho = _initial_guess[0];
    _kappa = _initial_guess[1];
    _theta = _initial_guess[2];
    _v0 = _initial_guess[3];
    _sigma = _initial_guess[4];

    std::cout << "Calibration done:\n";
    std::cout << "rho = " << _rho
        << ", kappa = " << _kappa
        << ", theta = " << _theta
        << ", v0 = " << _v0
        << ", sigma = " << _sigma << std::endl;
}

// Simple CSV/space-separated parser
//std::vector<std::vector<double>> Calibration::get_data(const char* path) {
//    std::ifstream file(path);
//    std::vector<std::vector<double>> data;
//    std::string line;
//
//    while (std::getline(file, line)) {
//        std::vector<double> row;
//        std::stringstream ss(line);
//        double val;
//
//        while (ss >> val) {
//            row.push_back(val);
//            if (ss.peek() == ',' || ss.peek() == ' ') ss.ignore();
//        }
//
//        data.push_back(row);
//    }
//
//    return data;
//}

// Getters
double Calibration::getRho() const { return _rho; }
double Calibration::getKappa() const { return _kappa; }
double Calibration::getTheta() const { return _theta; }
double Calibration::getV0() const { return _v0; }
double Calibration::getSigma() const { return _sigma; }
