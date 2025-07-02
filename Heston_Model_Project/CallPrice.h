#pragma once
#include "Model.h"
#include <complex>
#include <boost/math/quadrature/gauss.hpp>

class HestonPricer {
public:
    // Default Constructor
    HestonPricer() = default;
    // Constructor with parameters
    HestonPricer(const HestonModel&  heston_model);
    // Copy Constructor
	HestonPricer(const HestonPricer& heston_pricer);
    // Copy Assignment Operator
	HestonPricer& operator=(const HestonPricer& heston_pricer);
    // Destructor
	~HestonPricer() = default;

	// Compute Characteristic Function PHIi
	std::complex<double> CharacteristicFunction(double omega, double T, double time, double x, bool P1);
    // Compute the integrand in P1 if parameter P1==true else compute the integrand in P2
	double RealIntegrand(double u, double T, double time, double S0, double K, bool P1);
    // Compute P1 if parameter P1==true else compute P2 using Gauss Legendre quadrature
	double Pi(double T, double time, double S0, double K, bool P1);
    // Compute Heston call price 
	double CallHestonPrice(double T, double time, double S0, double K);

private:
    // Heston Model
	HestonModel _heston_model;
};

class CallOption {
public:
    // Constructor with parameters
    CallOption(double S, double K, double T, double r, double marketPrice);

    // Compute Black Scholes call price
    double call_price(double sigma) const;
    // Compute vega of Black Scholes call price
    double vega(double sigma) const;
    // Compute implied volatility inversing the call price using Newton Raphson method
    double compute_implied_vol(double initialGuess = 0.19, double tol = 1e-6, int maxIter = 100) const;

private:
    // Cumulative distribution function for a gaussian variable
    double norm_cdf(double x) const;

    // Spot price
    double _S;
    // Strike
    double _K;
    // Maturity
    double _T;
    // Risk free rate
    double _r;
    // Call market price
    double _marketPrice;
};