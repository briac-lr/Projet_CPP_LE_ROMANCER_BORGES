#include "RandomVariable.h"

// DANGER
//RandomVariable::RandomVariable(Distribution* dist_ptr)
//	: _distribution_ptr(dist_ptr)
//{}

RandomVariable::RandomVariable(const Distribution& distribution)
	: _distribution_ptr(distribution.clone())
{
}

RandomVariable::RandomVariable(const RandomVariable& random_variable)
	: _distribution_ptr(random_variable._distribution_ptr->clone())
{
}

RandomVariable& RandomVariable::operator=(const RandomVariable& random_variable)
{
	if (this != &random_variable)
	{
		_distribution_ptr = random_variable._distribution_ptr->clone();
	}
	return *this;
}

RandomVariable::~RandomVariable()
{
	delete _distribution_ptr;
}

double RandomVariable::expectation() const
{
	// int_{- infty}^{+ infty} (x * _distribution_ptr->pdf(x)) dx
	return 0.0;
}

double RandomVariable::variance() const
{
	// int_{- infty}^{+ infty} (x * x * _distribution_ptr->pdf(x)) dx - expectation()*expectation();
	return 0.0;
}
