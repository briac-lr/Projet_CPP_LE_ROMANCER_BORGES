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
	// Default Constructor
	BlackScholesModel() = default;
	// Constructor with parameters
	BlackScholesModel(const double& drift, const double& volatility);
	// Copy Constructor
	BlackScholesModel(const BlackScholesModel& model);
	// Copy Assignment Operator
	BlackScholesModel& operator=(const BlackScholesModel& model);
	// Destructor
	~BlackScholesModel() = default;
	// Method Clone
	BlackScholesModel* clone() const override;

	double drift_term(const double& time, const double& asset_price) const;
	double diffusion_term(const double& time, const double& asset_price) const;

private:
	// BS drift
	double _drift;
	// BS volatility
	double _volatility;
};


class HestonModel : public Model
{
public:
	// Default Constructor
	HestonModel() = default;
	// Constructor with parameters
	HestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, double r);
	// Copy Constructor
	HestonModel(const HestonModel& model);
	// Copy Assignment Operator
	HestonModel& operator=(const HestonModel& model);
	// Destructor
	~HestonModel() = default;
	// Method Clone
	HestonModel* clone() const override;

	double _kappa;
	double _theta;
	double _sigma;
	double _rho;
	double _v0;
	double _r;

private:

};