#include "Distribution.h"

void main()
{
	// An abstract class cannot be instantiated
	//Distribution distribution("My_Distribution");

	Distribution* distribution_ptr = new NormalDistribution("my_distr", 0., 1.);

	NormalDistribution norm_dist("Normal", 0., 1.);
	LogNormalDistribution lognorm_dist("LogNormal", 0., 1.);
}