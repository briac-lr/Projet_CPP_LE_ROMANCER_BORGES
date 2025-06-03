#ifndef MONTECARLO_H
#define MONTECARLO_H

// forward declaration
class PathSimulator;
class Payoff;

class MonteCarlo
{
public:
	MonteCarlo(const size_t num_sims, const PathSimulator& path_sim, const Payoff& payoff);

	double price() const;

private:
	size_t _number_simulations;
	const PathSimulator* _path_simulator;
	const Payoff* _payoff;

};


// 2 options for Payoff

// 1 - Inheritance for each type of payoff  XXXXX
// 2 - Class Payoff                         YES  













#endif // !MONTECARLO_H

