#include "Model.h"
#include <cmath>

// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ BS MODEL DEFINITION ------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Constructor with parameters
BlackScholesModel::BlackScholesModel(const double& drift, const double& volatility)
	: _drift(drift), _volatility(volatility)
{
}

// Copy constructor
BlackScholesModel::BlackScholesModel(const BlackScholesModel& model)
	: _drift(model._drift), _volatility(model._volatility)
{
}

// Copy assignment operator
BlackScholesModel& BlackScholesModel::operator=(const BlackScholesModel& model)
{
	if (this != &model)
	{
		_drift = model._drift;
		_volatility = model._volatility;
	}
	return *this;
}

// Method Clone
BlackScholesModel* BlackScholesModel::clone() const
{
	return new BlackScholesModel(*this);
}

// Drift term in BS
double BlackScholesModel::drift_term(const double& time, const double& asset_price) const
{
	return _drift * asset_price;
}

// Diffusion term in BS
double BlackScholesModel::diffusion_term(const double& time, const double& asset_price) const
{
	return _volatility * asset_price;
}

// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ HESTON MODEL DEFINITION ------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Constructor with parameters
HestonModel::HestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, double r)
    : _kappa(kappa), _theta(theta), _sigma(sigma), _rho(rho), _v0(v0), _r(r) 
{
}

// Copy constructor
HestonModel::HestonModel(const HestonModel& model)
    : _kappa(model._kappa), _theta(model._theta), _sigma(model._sigma), _rho(model._rho), _v0(model._v0), _r(model._r)
{
}

// Copy assignment operator
HestonModel& HestonModel::operator=(const HestonModel& model) {
    if (this != &model)
	{
        _kappa = model._kappa;
        _theta = model._theta;
        _sigma = model._sigma;
        _rho = model._rho;
        _v0 = model._v0;
        _r = model._r;
    }
    return *this;
}

// Method Clone
HestonModel* HestonModel::clone() const
{
	return new HestonModel(*this);
}