#include "CallPrice.h"
#include <complex>
#include <cmath>
#include <iostream>
#include <stdexcept>

// hard coding PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// For Gauss Legendre quadrature
using namespace boost::math::quadrature;

// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ HESTON PRICER DEFINITION -------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Constructor with parameters
HestonPricer::HestonPricer(const HestonModel& heston_model)
    : _heston_model(heston_model)
{
}

// Copy Constructor
HestonPricer::HestonPricer(const HestonPricer& heston_pricer)
    : _heston_model(heston_pricer._heston_model)
{
}

// Copy Assignment Operator
HestonPricer& HestonPricer::operator=(const HestonPricer& heston_pricer)
{
    if (this != &heston_pricer)
    {
        _heston_model = heston_pricer._heston_model;
    }
    return *this;
}

// Compute Characteristic Function PHIi
std::complex<double> HestonPricer::CharacteristicFunction(
    double omega, double T, double time, double x, bool P1) {

    // complex i
    std::complex<double> j(0.0, 1.0);

    double u_i;
    std::complex<double> y_i;

    if (P1 == true) {
        u_i = -1.0;
        y_i = j * omega - 1.0;
    }
    else {
        u_i = 1.0;
        y_i = j * omega;
    }

    std::complex<double> a = _heston_model._kappa - _heston_model._rho * _heston_model._sigma* y_i;
    std::complex<double> b = std::sqrt(a * a + _heston_model._sigma * _heston_model._sigma * (u_i * j * omega + omega * omega));
    std::complex<double> g = (a - b) / (a + b);

    std::complex<double> C = (j * omega * _heston_model._r * (T - time) +
                             (_heston_model._kappa * _heston_model._theta) / (_heston_model._sigma * _heston_model._sigma) *
                             ((a - b) * (T - time) - 2.0 * std::log((1.0 - g * std::exp(-b * (T - time))) / (1.0 - g)))
                             );

    std::complex<double> D = (a - b) / (_heston_model._sigma * _heston_model._sigma) * (1.0 - std::exp(-b * (T - time))) / (1.0 - g * std::exp(-b * (T - time)));

    return std::exp(C + D * _heston_model._v0 + j * omega * x);
}

// Compute the integrand in P1 if parameter P1==true else compute the integrand in P2
double HestonPricer::RealIntegrand(double u, double T, double time, double S0, double K, bool P1) {

    // Avoid division by zero
    if (u == 0.0) {
        return 0.0;
    }

    // Complex i
    std::complex<double> j(0.0, 1.0);

    // Variable change in the integral to go from -inf, +inf to -1, +1
    double variable_change = u / (1 - u * u);
    double numerator = 1 + u * u;
    double denominator = std::pow(1 - u * u, 2);

    double x = std::log(S0);

    std::complex<double> phi = CharacteristicFunction(variable_change, T, time, x, P1);

    std::complex<double> exp_term = std::exp(-j * variable_change * std::log(K));
    
    // Factor due to variable change
    double factor = numerator / denominator;

    // Compute complex value
    std::complex<double> integrand = factor * (phi * exp_term) / (j * variable_change);

    // Taking the real part
    return std::real(integrand);
}

// Compute P1 if parameter P1==true else compute P2 using Gauss Legendre quadrature
double HestonPricer::Pi(double T, double time, double S0, double K, bool P1) {

    // Number of points in the Gaussian Legendre quadrature
    const int N = 64;

    // Integrator object
    gauss<double, N> integrator;

    // The auto keyword enables the compiler to automatically deduce the type of a variable
    auto integrand = [&](double u) {
        return RealIntegrand(u, T, time, S0, K, P1);
        };

    // Integrate from -1 to 1
    double integral = integrator.integrate(integrand, -1.0, 1.0);

    // Return Pi
    return 0.5 + (1.0 / (2.0 * M_PI)) * integral;
}

// Compute Heston call price 
double HestonPricer::CallHestonPrice(double T, double time, double S0, double K) {

    // Compute P1
    double P1 = Pi(T, time, S0, K, true);
    // Compute P2
    double P2 = Pi(T, time, S0, K, false);

    // Finally compute the call price
    double call_price = S0 * P1 - K * std::exp(-_heston_model._r * T) * P2;

    return call_price;
}

// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ CALL OPTION DEFINITION ------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Constructor with parameters
CallOption::CallOption(double S, double K, double T, double r, double marketPrice)
    : _S(S), _K(K), _T(T), _r(r), _marketPrice(marketPrice) 
{
}

// Cumulative distribution function for a gaussian variable
double CallOption::norm_cdf(double x) const {
    return 0.5 * std::erfc(-x * 1 / std::sqrt(2));
}

// Compute Black Scholes price
double CallOption::call_price(double sigma) const {
    double d1 = (std::log(_S / _K) + (_r + 0.5 * sigma * sigma) * _T) / (sigma * std::sqrt(_T));
    double d2 = d1 - sigma * std::sqrt(_T);
    return _S * norm_cdf(d1) - _K * std::exp(-_r * _T) * norm_cdf(d2);
}

// Compute vega of Black Scholes call price
double CallOption::vega(double sigma) const {
    double d1 = (std::log(_S / _K) + (_r + 0.5 * sigma * sigma) * _T) / (sigma * std::sqrt(_T));
    return _S * std::sqrt(_T) * std::exp(-0.5 * d1 * d1) / std::sqrt(2 * M_PI);
}

// Compute implied volatility inversing the call price using Newton Raphson method
double CallOption::compute_implied_vol(double initialGuess, double tol, int maxIter) const
// initial guess : initial value for sigma
// tol : tolerance on the error
// maxIter : max number of iteration
{
    // Avoid division by zero
    const double VEGA_EPS = 1e-8;
    // Lower bound for sigma
    const double SIG_LOW = 1e-4;
    // Upper bound for sigma
    const double SIG_HIGH = 5.0;

    // Initial value for sigma
    double sigma = initialGuess;

    for (int i = 0; i < maxIter; ++i)
    {   
        // BS price
        double price = call_price(sigma);
        double diff = price - _marketPrice;

        // if abs(diff)< tol -> return sigma
        if (std::fabs(diff) < tol)
            return sigma;

        // Avoid division by zero
        double v = vega(sigma);
        if (std::fabs(v) < VEGA_EPS)
            break;

        // Newton step
        sigma -= diff / v;

        // Lower bound for sigma
        if (sigma <= 0.0)
            sigma = SIG_LOW;
    }

    
    // Fallback: bissection method
    double low = SIG_LOW;
    double high = SIG_HIGH;

    // 100 bisection steps
    for (int i = 0; i < 100; ++i)
    {
        sigma = 0.5 * (low + high);
        double diff = call_price(sigma) - _marketPrice;

        if (std::fabs(diff) < tol)
            return sigma;

        if (diff > 0.0)
            high = sigma;
        else
            low = sigma;
    } 
    
    throw std::runtime_error("Implied volatility not found (Newton + bisection failed)");
}
