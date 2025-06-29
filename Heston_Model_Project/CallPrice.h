#pragma once

#include "Model.h"

#include <complex>
#include <boost/math/quadrature/gauss.hpp>

class HestonPricer {
public:
    HestonPricer(const HestonModel&  heston_model);
	HestonPricer(const HestonPricer& heston_pricer);
	HestonPricer& operator=(const HestonPricer& heston_pricer);
	~HestonPricer() = default;

	// Compute Characteristic Function PHI_i
	std::complex<double> CharacteristicFunction(double omega, double T, double time, double x, bool P1);
	double RealIntegrand(double u, double T, double time, double S0, double K, bool P1);
	double Pi(double T, double time, double S0, double K, bool P1);
	double CallHestonPrice(double T, double time, double S0, double K);
	double ImpliedVolatility(double T, double time, double S0, double K, double CallPrice);
private:
	HestonModel _heston_model;
};

class CallOption {
public:
    CallOption(double S, double K, double T, double r, double marketPrice);

    double call_price(double sigma) const;
    double vega(double sigma) const;
    double compute_implied_vol(double initialGuess = 0.2, double tol = 1e-6, int maxIter = 100) const;

private:
    double norm_cdf(double x) const;

    // Spot price
    double S;
    // Strike
    double K;
    // Maturity
    double T;
    // Risk free rate
    double r;
    // Call price
    double marketPrice;
};