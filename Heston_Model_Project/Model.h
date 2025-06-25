#pragma once

#include <complex>

// Abstract base class - represents all one-dimensional Ito Processes
class Model {
public:

	virtual Model* clone() const = 0;

	virtual ~Model() = default;

protected:

};

// Implementation of the Black Scholes model
class BlackScholesModel : public Model
{
public:
	BlackScholesModel(const double& drift, const double& volatility);
	BlackScholesModel(const BlackScholesModel& model);
	BlackScholesModel& operator=(const BlackScholesModel& model);

	double drift_term(const double& time, const double& asset_price) const;
	double diffusion_term(const double& time, const double& asset_price) const;

	BlackScholesModel* clone() const override;

	~BlackScholesModel() = default;

private:
	double _drift;        // BS drift
	double _volatility;   // BS volatility
};


class HestonModel : public Model
{
public:

	// Constructor with parameters
	HestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, double r);

	// Copy Constructor
	HestonModel(const HestonModel& model);

	// Copy Assignment Operator
	HestonModel& operator=(const HestonModel& model);

	// Destructor
	~HestonModel() = default;

	//double drift_term(const double& time, const double& asset_price) const override;
	//double diffusion_term(const double& time, const double& asset_price) const override;
	HestonModel* clone() const override;

	double _kappa;
	double _theta;
	double _sigma;
	double _rho;
	double _v0;
	double _r;

private:

};