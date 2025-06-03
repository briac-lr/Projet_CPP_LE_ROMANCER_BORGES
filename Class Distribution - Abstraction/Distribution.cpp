#include "Distribution.h"

Distribution::Distribution(const std::string name)
	: _name(name)
{
}

Distribution::Distribution(const Distribution& distribution)
	: _name(distribution._name)
{
}

Distribution& Distribution::operator=(const Distribution& distribution)
{
	if (this != &distribution)
	{
		_name = distribution._name;
	}
	return *this;
}

double Distribution::cdf(double y) const
{
	// \int_{- infty}^{y} pdf(x) dx;
	return 0.0;
}

NormalDistribution::NormalDistribution(const std::string name, const double& mean, const double& variance)
	: Distribution(name), _mean(mean), _variance(variance)
{
}

NormalDistribution::NormalDistribution(const NormalDistribution& distribution)
	: Distribution(distribution), _mean(distribution._mean), _variance(distribution._variance)
{
}

NormalDistribution& NormalDistribution::operator=(const NormalDistribution& distribution)
{
	if (this != &distribution)
	{
		Distribution::operator=(distribution);
		_mean = distribution._mean;
		_variance = distribution._variance;
	}
	return *this;
}

double NormalDistribution::pdf(double x) const
{
	// TO DO : implement the formula N(mu, sigma^2)
	return 0.0;
}

NormalDistribution* NormalDistribution::clone() const
{
	return new NormalDistribution(*this);
}

LogNormalDistribution::LogNormalDistribution(const std::string name, const double& mean, const double& variance)
	: Distribution(name), _mean(mean), _variance(variance)
{
}

LogNormalDistribution::LogNormalDistribution(const LogNormalDistribution& distribution)
	: Distribution(distribution), _mean(distribution._mean), _variance(distribution._variance)
{
}

LogNormalDistribution& LogNormalDistribution::operator=(const LogNormalDistribution& distribution)
{
	if (this != &distribution)
	{
		Distribution::operator=(distribution);
		_mean = distribution._mean;
		_variance = distribution._variance;
	}
	return *this;
}

double LogNormalDistribution::pdf(double x) const
{
	// TO DO : implement the formula LN(mu, sigma^2)
	return 0.0;
}

LogNormalDistribution* LogNormalDistribution::clone() const
{
	return new LogNormalDistribution(*this);
}
