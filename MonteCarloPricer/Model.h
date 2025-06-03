#ifndef MODEL_H
#define MODEL_H

#include "ImpliedVolatilitySurface.h"

// Abstract class - represents all one-dimensional Ito Processes
class Model {
public:
	
	virtual double drift_term(const double& time, const double& asset_price) const = 0;
	virtual double diffusion_term(const double& time, const double& asset_price) const = 0;

	virtual Model* clone() const = 0;

	virtual ~Model() = default;

protected:

};

class BlackScholesModel : public Model
{
public:
	BlackScholesModel(const double& drift, const double& volatility);
	BlackScholesModel(const BlackScholesModel& model);
	BlackScholesModel& operator=(const BlackScholesModel& model);

	double drift_term(const double& time, const double& asset_price) const override;
	double diffusion_term(const double& time, const double& asset_price) const override;
	
	BlackScholesModel* clone() const override;

	~BlackScholesModel() = default;

private:
	double _drift;        // BS drift
	double _volatility;   // BS volatility
};

class DupireLocalVolatilityModel : public Model
{
public:
	DupireLocalVolatilityModel(
		const ImpliedVolatilitySurface& implied_vol, 
		const double& eps_time, 
		const double& eps_asset);

	double drift_term(const double& time, const double& asset_price) const override;
    double diffusion_term(const double& time, const double& asset_price) const override;
	DupireLocalVolatilityModel* clone() const override;

private:
	// Method to compute sigma_D(t,s)
	double dupire_local_volatility(const double& time, const double& asset_price) const;

	double first_order_derivative_implied_vol_time(const double& time, const double& asset_price) const;
	double first_order_derivative_implied_vol_asset(const double& time, const double& asset_price) const;
	double second_order_derivative_implied_vol_asset(const double& time, const double& asset_price) const;

	ImpliedVolatilitySurface _implied_volatility_surface;
	double _epsilon_time;
	double _epsilon_asset;
};




#endif // !MODEL_H

