#include "CSVReader.h"
#include "Model.h"
#include "Calibration.h"
#include "CallPrice.h"

#include "iostream"
#include <cstring>
#include <vector>

#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>

// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ HESTON MODEL CALIBRATION DEFINITION --------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Parameterized constructor
Calibration::Calibration(
    const std::array<double, 
    5>& initial_guess, 
    int iterations, 
    const std::string& market_data_path,
    const double r,
    const double S0,
    const double time)
    : _initial_guess(initial_guess), 
    _number_iterations(iterations),
    _market_data_path(market_data_path),
    _rho(0), _kappa(0), _theta(0), _v0(0), _sigma(0), _r(r), _S0(S0), _time(time)
{
    CSVReader reader(_market_data_path);
    _market_data = reader.read();
}

// Copy constructor
Calibration::Calibration(const Calibration& other)
    :_initial_guess(other._initial_guess),
    _number_iterations(other._number_iterations), 
    _market_data_path(other._market_data_path),
    _rho(other._rho), _kappa(other._kappa), _theta(other._theta), _v0(other._v0), _sigma(other._sigma),
    _r(other._r), _S0(other._S0), _time(other._time)
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
        _r = other._r;
        _S0 = other._S0;
        _time = other._time;
    }
    return *this;
}

// Destructor
Calibration::~Calibration() 
{

}

// Public method to run the calibration
void Calibration::Calibrate()
{
    // Building vector of initial values for Heston parameters
    Eigen::VectorXd x(5);
    for (int i = 0; i < 5; ++i) x(i) = _initial_guess[i];

    // This functor is used with Eigen's Levenberg-Marquardt solver to calibrate 
    // the 5 - parameter Heston model against market volatilities.
    HestonCalibrationFunctor functor(_market_data, _r, _S0, _time);
    // Automatic differentiation
    Eigen::NumericalDiff<HestonCalibrationFunctor> numDiff(functor);
    // Eigen Levenberg-Marquardt solver
    Eigen::LevenbergMarquardt<Eigen::NumericalDiff<HestonCalibrationFunctor>, double> lm(numDiff);

    // Applying minimization
    lm.parameters.maxfev = _number_iterations;
    lm.minimize(x);

    // Calibrated parameters
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


// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ CALIBRATION FUNCTOR EIGEN DEFINITION -------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Constructor with parameters
HestonCalibrationFunctor::HestonCalibrationFunctor(
    const std::vector<std::vector<double>>& market_data,
    const double r,
    const double S0,
    const double time)
    : _data(market_data), _r(r), _S0(S0), _time(time)
{

}

// Method to compute the residuals
int HestonCalibrationFunctor::operator()(const Eigen::VectorXd& x,Eigen::VectorXd& fvec) const
{
    const double rho = x(0);
    const double kappa = x(1);
    const double theta = x(2);
    const double v0 = x(3);
    const double sigma = x(4);

    for (int i = 0; i < values(); ++i) {
        const double market_vol = _data[i][1];
        const double K = _data[i][2];
        const double T = _data[i][3];

        HestonModel model(kappa, theta, sigma, rho, v0, _r);

        // Heston pricer using constructor with parameters
        HestonPricer pricer(model);

        // Compute Heston Call price
        double call_price = pricer.CallHestonPrice(T, _time, _S0, K);
        if (call_price <= 0.0) {
            std::cerr << "Invalid call price: " << call_price << " (K=" << K << ", T=" << T << ")\n";
        }
        CallOption call(_S0, K, T, _r, call_price);

        // Compute implied volatility of the call price
        try
        {
            double HestonImpliedVol = call.compute_implied_vol();
            fvec(i) = HestonImpliedVol - market_vol;      // résidu normal
        }
        catch (const std::exception& ex)
        {
            std::cerr << "SKIP: " << ex.what()<< "  (K=" << K << ", T=" << T << ")\n";

            // Adaptative penality if computing implied vol failed
            fvec(i) = 2.0 * market_vol;
            continue;
        }

    }
    return 0;
}

