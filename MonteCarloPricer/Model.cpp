#include "Model.h"


BlackScholesModel::BlackScholesModel(const double& drift, const double& volatility)
	: _drift(drift), _volatility(volatility)
{
}

BlackScholesModel::BlackScholesModel(const BlackScholesModel& model)
	: _drift(model._drift), _volatility(model._volatility)
{
}

BlackScholesModel& BlackScholesModel::operator=(const BlackScholesModel& model)
{
	if (this != &model)
	{
		_drift = model._drift;
		_volatility = model._volatility;
	}
	return *this;
}

double BlackScholesModel::drift_term(const double& time, const double& asset_price) const
{
	return _drift * asset_price;
}

double BlackScholesModel::diffusion_term(const double& time, const double& asset_price) const
{
	return _volatility * asset_price;
}

BlackScholesModel* BlackScholesModel::clone() const
{
	return new BlackScholesModel(*this);
}


DupireLocalVolatilityModel::DupireLocalVolatilityModel(
	const ImpliedVolatilitySurface& implied_vol,
	const double& eps_time,
	const double& eps_asset)
	: _implied_volatility_surface(implied_vol), 
	  _epsilon_time(eps_time), 
	  _epsilon_asset(eps_asset)
{
}

double DupireLocalVolatilityModel::drift_term(const double& time, const double& asset_price) const
{
	return _implied_volatility_surface.risk_free_rate() * asset_price;
}

double DupireLocalVolatilityModel::diffusion_term(const double& time, const double& asset_price) const
{
	return dupire_local_volatility(time, asset_price) * asset_price;
}

DupireLocalVolatilityModel* DupireLocalVolatilityModel::clone() const
{
	return new DupireLocalVolatilityModel(*this);
}

double DupireLocalVolatilityModel::dupire_local_volatility(const double& time, const double& asset_price) const
{
	// TO DO 
	return 0.0;
}

double DupireLocalVolatilityModel::first_order_derivative_implied_vol_time(const double& time, const double& asset_price) const
{
	// sigma_up   :   sigma^*(time + epsilon_time, asset_price)
	double sigma_up = _implied_volatility_surface.implied_volatility(time + _epsilon_time, asset_price);
	// sigma_down :   sigma^*(time - epsilon_time, asset_price)
	double sigma_down = _implied_volatility_surface.implied_volatility(time - _epsilon_time, asset_price);

	double dsigma_dt = 0.5 * (sigma_up - sigma_down) / _epsilon_time;
	return dsigma_dt;
}

double DupireLocalVolatilityModel::first_order_derivative_implied_vol_asset(const double& time, const double& asset_price) const
{
	// sigma_up   :   sigma^*(time, asset_price + epsilon_asset)
	double sigma_up = _implied_volatility_surface.implied_volatility(time, asset_price + _epsilon_asset);
	// sigma_down :   sigma^*(time, asset_price - epsilon_asset)
	double sigma_down = _implied_volatility_surface.implied_volatility(time, asset_price - _epsilon_asset);

	double dsigma_ds = 0.5 * (sigma_up - sigma_down) / _epsilon_asset;
	return dsigma_ds;
}

double DupireLocalVolatilityModel::second_order_derivative_implied_vol_asset(const double& time, const double& asset_price) const
{
	// sigma_up   :   sigma^*(time, asset_price + epsilon_asset)
	double sigma_up = _implied_volatility_surface.implied_volatility(time, asset_price + _epsilon_asset);
	// sigma_down :   sigma^*(time, asset_price - epsilon_asset)
	double sigma_down = _implied_volatility_surface.implied_volatility(time, asset_price - _epsilon_asset);
	// sigma      :   sigma^*(time, asset_price)
	double sigma = _implied_volatility_surface.implied_volatility(time, asset_price);

	double d2sigma_ds2 = (sigma_up - 2. * sigma + sigma_down) / (_epsilon_asset* _epsilon_asset);
	return d2sigma_ds2;
}


