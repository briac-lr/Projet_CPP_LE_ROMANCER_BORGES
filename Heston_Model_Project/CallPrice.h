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

private:
	HestonModel _heston_model;
};