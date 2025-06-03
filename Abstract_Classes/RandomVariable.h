#pragma once
#include "Distribution.h"

class RandomVariable
{
public:
	RandomVariable(const Distribution& distribution);

private:
	const Distribution* _distribution_ptr;
};