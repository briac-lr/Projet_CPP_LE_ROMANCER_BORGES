#pragma once
#include "Distribution.h"

// forward declaration
//class Distribution;

class RandomVariable
{
public:
	//RandomVariable(Distribution* dist_ptr);
	RandomVariable(const Distribution& distribution);
	RandomVariable(const RandomVariable& random_variable);
	RandomVariable& operator=(const RandomVariable& random_variable);
	~RandomVariable();

	double expectation() const;
	double variance() const;

private:
	Distribution* _distribution_ptr;
};