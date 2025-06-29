#include "Calibration.h"
#include "iostream"
#include <cstring>
#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>
// #include "CSVReader.h"
#include "Model.h"


// Parameterized constructor   
Calibration::Calibration(const std::array<double, 5>& initial_guess, int iterations, const std::string& market_data_path)
    : _initial_guess(initial_guess), _number_iterations(iterations), _market_data_path(market_data_path),
    _rho(0), _kappa(0), _theta(0), _v0(0), _sigma(0)
{

}

// Copy constructor
Calibration::Calibration(const Calibration& other)
    :_initial_guess(other._initial_guess),
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
}

void Calibration::Calibrate() {
    std::vector<std::vector<double>> data = get_data(_market_data_path);

    Eigen::VectorXd x(5);
    for (int i = 0; i < 5; ++i)
        x(i) = _initial_guess[i];

    HestonCalibrationFunctor functor(data);
    Eigen::NumericalDiff<HestonCalibrationFunctor> numDiff(functor);
    Eigen::LevenbergMarquardt<Eigen::NumericalDiff<HestonCalibrationFunctor>> lm(numDiff);

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


// Ce functor est utilisé avec Levenberg-Marquardt d’Eigen
class HestonCalibrationFunctor : public Eigen::DenseFunctor<double> {
    const std::vector<std::vector<double>>& market_data;

public:
    // Constructeur : 5 paramètres à calibrer, N points de données
    HestonCalibrationFunctor(const std::vector<std::vector<double>>& data)
        : Eigen::DenseFunctor<double>(5, static_cast<int>(data.size())), market_data(data) {}

    int operator()(const Eigen::VectorXd& x, Eigen::VectorXd& fvec) const override {
        double rho = x(0);
        double kappa = x(1);
        double theta = x(2);
        double v0 = x(3);
        double sigma = x(4);

        for (int i = 0; i < this->values(); ++i) {
            double K = market_data[i][0]; // Strike
            double T = market_data[i][1]; // Maturité
            double market_vol = market_data[i][2]; // Vol observée

            //  Cette fonction doit être définie ailleurs (ton modèle Heston)
            double model_vol = compute_model_volatility(K, T, rho, kappa, theta, v0, sigma);

            fvec(i) = model_vol - market_vol;
        }
        return 0;
    }
};
