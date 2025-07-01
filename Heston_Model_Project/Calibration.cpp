#include "iostream"
#include <cstring>
#include <vector>

#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>

#include "CSVReader.h"
#include "Model.h"
#include "Calibration.h"
#include "CallPrice.h"


// Parameterized constructor
Calibration::Calibration(const std::array<double, 5>& initial_guess,
    int iterations,
    const std::string& market_data_path)
    : _initial_guess(initial_guess),
    _number_iterations(iterations),
    _market_data_path(market_data_path),
    _rho(0), _kappa(0), _theta(0), _v0(0), _sigma(0)
{
    CSVReader reader(_market_data_path);
    _market_data = reader.read();   // may throw if file is bad
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
Calibration::~Calibration() 
{

}

void Calibration::Calibrate()
{
    Eigen::VectorXd x(5);
    for (int i = 0; i < 5; ++i) x(i) = _initial_guess[i];

    HestonCalibrationFunctor functor(_market_data);
    Eigen::NumericalDiff<HestonCalibrationFunctor> numDiff(functor);
    Eigen::LevenbergMarquardt<
        Eigen::NumericalDiff<HestonCalibrationFunctor>, double> lm(numDiff);

    lm.parameters.maxfev = _number_iterations;
    lm.minimize(x);

    _rho = x(0);
    _kappa = x(1);
    _theta = x(2);
    _v0 = x(3);
    _sigma = x(4);

}

// Getters
double Calibration::getRho() const { return _rho; }
double Calibration::getKappa() const { return _kappa; }
double Calibration::getTheta() const { return _theta; }
double Calibration::getV0() const { return _v0; }
double Calibration::getSigma() const { return _sigma; }


HestonCalibrationFunctor::HestonCalibrationFunctor(
    const std::vector<std::vector<double>>& market_data)
    : _data(market_data) 
{

}

int HestonCalibrationFunctor::operator()(const Eigen::VectorXd& x,
    Eigen::VectorXd& fvec) const
{
    const double rho = x(0);
    const double kappa = x(1);
    const double theta = x(2);
    const double v0 = x(3);
    const double sigma = x(4);

    const double r = 0.0423;
    const double time = 0.0;
    const double S0 = 498.63;

    for (int i = 0; i < values(); ++i) {
        std::cout << i << std::endl;
        const double market_vol = _data[i][1];  // observed vol
        const double K = _data[i][2];   // strike
        const double T = _data[i][3];   // maturity

        HestonModel model(kappa, theta, sigma, rho, v0, r);

        // Heston pricer using constructor with parameters
        HestonPricer pricer(model);

        // Compute Heston Call price
        double call_price = pricer.CallHestonPrice(T, time, S0, K);

        // Call price using constructor with parameters
        CallOption call(S0, K, T, r, call_price);

        // Compute implied volatility of the call price

        try
        {
            double HestonImpliedVol = call.compute_implied_vol();
            fvec(i) = HestonImpliedVol - market_vol;      // résidu normal
        }
        catch (const std::exception& ex)
        {
            std::cerr << "SKIP: " << ex.what()
                << "  (K=" << K << ", T=" << T << ")\n";

            /* Résidu neutre : n’influence pas la somme des carrés */
            fvec(i) = 0.0;
            continue;                       // passe au point suivant
        }

    }
    return 0;   // success
}

