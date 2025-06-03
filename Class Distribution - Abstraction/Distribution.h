#pragma once

#include <string>

// Normal, Lognormal, t-student, Uniform

// An abstract class is a class which has at least one pure virtual method 
class Distribution
{
public:
	Distribution(const std::string name);
	Distribution(const Distribution& distribution);
	Distribution& operator=(const Distribution& distribution);
	virtual ~Distribution() = default;

	// probability distribution function
	virtual double pdf(double x) const = 0; // pure virtual method

	// Cloning and return a pointer to the copy
	virtual Distribution* clone() const = 0;

	double cdf(double y) const;

protected:
	std::string _name;
};

class NormalDistribution : public Distribution
{
public:
	NormalDistribution(const std::string name, const double& mean, const double& variance);
	NormalDistribution(const NormalDistribution& distribution);
	NormalDistribution& operator=(const NormalDistribution& distribution);
	~NormalDistribution() = default;

	// We must re-implement the pure virtual methods of the base class
	double pdf(double x) const override;

	NormalDistribution* clone() const override;

private:
	double _mean;
	double _variance;
};

class LogNormalDistribution : public Distribution
{
public:
	LogNormalDistribution(const std::string name, const double& mean, const double& variance);
	LogNormalDistribution(const LogNormalDistribution& distribution);
	LogNormalDistribution& operator=(const LogNormalDistribution& distribution);
	~LogNormalDistribution() = default;

	// We must re-implement the pure virtual methods of the base class
	double pdf(double x) const override;

	LogNormalDistribution* clone() const override;

private:
	double _mean;
	double _variance;
};