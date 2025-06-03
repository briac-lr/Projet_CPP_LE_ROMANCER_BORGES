#pragma once

#include <string>

// Abstract class = has at least 1 pure virtual method
class Distribution
{
public:
	virtual double pdf(double x) const = 0; // pure virtual 
	
	// Delegate the copy construction to the derived classes and returns a pointer to the copied object
	virtual Distribution* clone() const = 0;

protected:
	Distribution(const std::string& name);
	Distribution(const Distribution& dist);
	std::string _name;
};

class NormalDistribution : public Distribution
{
public:
	NormalDistribution(const std::string& name, const double& mean, const double& variance);
	NormalDistribution(const NormalDistribution& dist);
	
	// We have to reimplement all base class virtual pure methods
	double pdf(double x) const override;
	NormalDistribution* clone() const override;


private:
	double _mean;
	double _variance;

};

class LogNormalDistribution : public Distribution
{
public:
	LogNormalDistribution(const std::string& name, const double& mean, const double& variance);
	LogNormalDistribution(const LogNormalDistribution& dist);

	double pdf(double x) const override;
	LogNormalDistribution* clone() const override;

private:
	double _mean;
	double _variance;

};