#pragma once

#include <vector>
#include <string>
#include <array>
#include <Eigen/Dense>


class Calibration{
public:
    // Parameterized constructor
    Calibration(const std::array<double, 5>& initial_guess, int iterations, const std::string& market_data_path);

    // Copy constructor
    Calibration(const Calibration& other);

    // Copy assignment operator
    Calibration& operator=(const Calibration& other);

    // Destructor
    ~Calibration();

    // Public method to run the calibration
    void Calibrate();

    // Accessors for calibrated parameters
    double getRho() const;
    double getKappa() const;
    double getTheta() const;
    double getV0() const;
    double getSigma() const;

private:
    std::array<double, 5> _initial_guess;
    int _number_iterations;
    std::string _market_data_path; // absolute path
    std::vector<std::vector<double>> _market_data;
    double _rho, _kappa, _theta, _v0, _sigma;

};

// Ce functor est utilisé avec Levenberg-Marquardt d’Eigen
/**
 *  Functor used by Eigen's Levenberg-Marquardt solver to calibrate the
 *  5-parameter Heston model against market volatilities.
 *
 *  It does **not** inherit from Eigen::DenseFunctor; instead we expose the
 *  minimal static/virtual interface expected by Eigen's LM implementation.
 */
class HestonCalibrationFunctor
{
public:
    /* =====   Traits required by LevenbergMarquardt   ===== */
    using Scalar = double;
    using InputType = Eigen::VectorXd;       // length 5
    using ValueType = Eigen::VectorXd;       // length = #data lines
    using JacobianType = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;

    enum {
        InputsAtCompileTime = 5,        // rho, kappa, theta, v0, sigma
        ValuesAtCompileTime = Eigen::Dynamic
    };

    /* =====   Ctor & interface expected by NumericalDiff   ===== */
    explicit HestonCalibrationFunctor(const std::vector<std::vector<double>>& market_data);

    /**
     *  Compute residuals:  model_vol(K,T) – market_vol
     *  @param x     5-vector of Heston parameters
     *  @param fvec  residuals, sized  = values()
     *  @return      0 on success   (Eigen convention)
     */
    int operator()(const Eigen::VectorXd& x, Eigen::VectorXd& fvec) const;

    /* These two are queried by NumericalDiff / LM */
    int inputs()  const { return InputsAtCompileTime; }
    int values()  const { return static_cast<int>(_data.size()); }

private:
    const std::vector<std::vector<double>>& _data;   // strike, maturity, vol
};