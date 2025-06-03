#include "Distribution.h"
#include "RandomVariable.h"

int main()
{
	// Distribution dist("My_Dist");
	NormalDistribution norm("Normal", 0., 1.);
	LogNormalDistribution lognorm("LogNormal", 2., 5.);

	Distribution* norm_ptr = new NormalDistribution("Normal", -2., 3.);
	Distribution* lognorm_ptr = new LogNormalDistribution("LogNormal", -1., 4.);

	double norm_density_at_zero = norm_ptr->pdf(0.);
	double lognorm_density_at_zero = lognorm_ptr->pdf(0.);

	RandomVariable norm_var(norm);
	RandomVariable lognorm_var(lognorm);

	return 0;
}