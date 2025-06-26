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

template<typename T>
struct HestonCalibrationFunctor : Eigen::DenseFunctor<T> {
    const std::vector<std::vector<double>>& market_data; // [strike, maturity, market_vol]

    HestonCalibrationFunctor(const std::vector<std::vector<double>>& data)
        : Eigen::DenseFunctor<T>(5, data.size()), market_data(data) {}

    // Model function
    int operator()(const Eigen::VectorXd& x, Eigen::VectorXd& fvec) const {
        double rho = x(0);
        double kappa = x(1);
        double theta = x(2);
        double v0 = x(3);
        double sigma = x(4);

        for (size_t i = 0; i < market_data.size(); ++i) {
            double K = market_data[i][0];     // strike
            double T = market_data[i][1];     // maturity
            double market_vol = market_data[i][2];

            double model_vol = compute_model_volatility(K, T, rho, kappa, theta, v0, sigma); // implement this
            fvec(i) = model_vol - market_vol;
        }
        return 0;
    }
};

Eigen::NumericalDiff<HestonCalibrationFunctor<double>> numDiff(functor);
Eigen::LevenbergMarquardt<Eigen::NumericalDiff<HestonCalibrationFunctor<double>>> lm(numDiff);

void Calibration::Calibrate() {
    std::vector<std::vector<double>> data = get_data(_market_data_path);

    Eigen::VectorXd x(5);
    for (int i = 0; i < 5; ++i)
        x(i) = _initial_guess[i];

    HestonCalibrationFunctor<double> functor(data);
    Eigen::NumericalDiff<HestonCalibrationFunctor<double>> numDiff(functor);
    Eigen::LevenbergMarquardt<Eigen::NumericalDiff<HestonCalibrationFunctor<double>>> lm(numDiff);
    lm.parameters.maxfev = _number_iterations;
    lm.minimize(x);

    _rho = x(0);
    _kappa = x(1);
    _theta = x(2);
    _v0 = x(3);
    _sigma = x(4);

    std::cout << "Calibration done:\n";
    std::cout << "rho = " << _rho
        << ", kappa = " << _kappa
        << ", theta = " << _theta
        << ", v0 = " << _v0
        << ", sigma = " << _sigma << std::endl;
}


// Getters
double Calibration::getRho() const { return _rho; }
double Calibration::getKappa() const { return _kappa; }
double Calibration::getTheta() const { return _theta; }
double Calibration::getV0() const { return _v0; }
double Calibration::getSigma() const { return _sigma; }
