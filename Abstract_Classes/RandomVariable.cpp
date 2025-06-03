#include "RandomVariable.h"

RandomVariable::RandomVariable(const Distribution& distribution)
//	: _distribution_ptr(&distribution) // DANGER : you could have a dangling pointer
	:_distribution_ptr(distribution.clone())
{
}
