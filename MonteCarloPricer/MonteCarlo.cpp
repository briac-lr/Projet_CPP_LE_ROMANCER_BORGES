#include "MonteCarlo.h"
#include "PathSimulator.h"
#include "Payoff.h"


MonteCarlo::MonteCarlo(const size_t num_sims, const PathSimulator& path_sim, const Payoff& payoff)
    : _number_simulations(num_sims), _path_simulator(path_sim.clone()), _payoff(payoff.clone())
{
}

double MonteCarlo::price() const
{
    double price = 0.;
    for (size_t sim_idx = 0; sim_idx < _number_simulations; sim_idx++)
        price += _payoff->discounted_payoff(_path_simulator->path(), _path_simulator->time_points());
    price /= (double)_number_simulations;

    return price;
}
