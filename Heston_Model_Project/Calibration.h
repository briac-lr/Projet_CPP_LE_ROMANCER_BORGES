#pragma once

#include <vector>
#include <string>
#include <array>
#include <Eigen/Dense>


class Calibration{
public:
    // Default Constructor
    Calibration() = default;

    // Constructor with parameters
    Calibration(
        const std::array<double, 5>& initial_guess, 
        int iterations, 
        const std::string& market_data_path,
        const double r,
        const double S0,
        const double time);

    // Copy constructor
    Calibration(const Calibration& other);

    // Copy assignment operator
    Calibration& operator=(const Calibration& other);

    // Destructor
    ~Calibration();

    // Public method to run the calibration
    void Calibrate();

    // Get calibrated parameters
    double getRho() const;
    double getKappa() const;
    double getTheta() const;
    double getV0() const;
    double getSigma() const;

private:
    // Initial values for rho, kappa, theta, v0, sigma
    std::array<double, 5> _initial_guess;
    // Number of iterations
    int _number_iterations;
    // Data file path
    std::string _market_data_path;
    // Data
    std::vector<std::vector<double>> _market_data;
    // Heston Parameters
    double _rho, _kappa, _theta, _v0, _sigma, _r, _S0, _time;

};

// This functor is used with Eigen's Levenberg-Marquardt solver to calibrate 
// the 5 - parameter Heston model against market volatilities.
class HestonCalibrationFunctor
{
public:
    // Constructor with parameters
    explicit HestonCalibrationFunctor(const std::vector<std::vector<double>>& market_data, 
        const double r, 
        const double S0, 
        const double time);

    // Heston parameters
    using InputType = Eigen::VectorXd;
    // Input data
    using ValueType = Eigen::VectorXd;
    // Requeried by NumericalDiff
    using Scalar = double;
    using JacobianType = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;

    // enum in C++ defines named constant values
    enum {
        // rho, kappa, theta, v0, sigma
        InputsAtCompileTime = 5,
        ValuesAtCompileTime = Eigen::Dynamic
    };

    // Compute residuals
    int operator()(const Eigen::VectorXd& x, Eigen::VectorXd& fvec) const;

    // Requeried by NumericalDiff
    int inputs()  const { return InputsAtCompileTime; }
    int values()  const { return static_cast<int>(_data.size()); }

private:
    // strike, maturity, vol
    const std::vector<std::vector<double>>& _data;
    const double _r;
    const double _S0;
    const double _time;
};