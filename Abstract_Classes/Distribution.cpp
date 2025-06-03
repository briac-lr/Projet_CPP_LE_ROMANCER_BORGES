#include "Distribution.h"

Distribution::Distribution(const std::string& name)
	: _name(name)
{
}

Distribution::Distribution(const Distribution& dist)
	:_name(dist._name)
{
}

NormalDistribution::NormalDistribution(const std::string& name, const double& mean, const double& variance)
	: Distribution(name), _mean(mean), _variance(variance)
{
}

NormalDistribution::NormalDistribution(const NormalDistribution& dist)
	: Distribution(dist), _mean(dist._mean), _variance(dist._variance)
{
}

double NormalDistribution::pdf(double x) const
{
	// 1/sqrt(2 pi sigma^2) exp(-(x-mean)^2/(2 sigma^2))
	return 0.0;
}

NormalDistribution* NormalDistribution::clone() const
{
	return new NormalDistribution(*this);
}

LogNormalDistribution::LogNormalDistribution(const std::string& name, const double& mean, const double& variance)
	: Distribution(name), _mean(mean), _variance(variance)
{
}

LogNormalDistribution::LogNormalDistribution(const LogNormalDistribution& dist)
	: Distribution(dist), _mean(dist._mean), _variance(dist._variance)
{
}

double LogNormalDistribution::pdf(double x) const
{
	// return the corresponding formula for the lognormal distribution
	return 0.0;
}

LogNormalDistribution* LogNormalDistribution::clone() const
{
	return new LogNormalDistribution(*this);
}
