#include "CallPrice.h"
#include <complex>
#include <cmath>

HestonPricer::HestonPricer(const HestonModel& heston_model)
    : _heston_model(heston_model)
{
}

HestonPricer::HestonPricer(const HestonPricer& heston_pricer)
    : _heston_model(heston_pricer._heston_model)
{
}

HestonPricer& HestonPricer::operator=(const HestonPricer& heston_pricer)
{
    if (this != &heston_pricer)
    {
        _heston_model = heston_pricer._heston_model;
    }
    return *this;
}



// Characteristic function 
std::complex<double> HestonPricer::CharacteristicFunction(
    double omega, double T, double time, double x, bool P1) {

    std::complex<double> j(0.0, 1.0);
    double u_i;
    std::complex<double> y_i;

    if (P1 == true) {
        u_i = -1;
        y_i = j * omega - 1.0;
    }
    else {
        u_i = 1;
        y_i = j * omega;
    }

    std::complex<double> a = _heston_model._kappa - _heston_model._rho * _heston_model._sigma* y_i;
    std::complex<double> b = std::sqrt(a * a + _heston_model._sigma * _heston_model._sigma * (u_i * j * omega + omega * omega));
    std::complex<double> g = (a - b) / (a + b);

    std::complex<double> num = _heston_model._r * j * omega * (T - time) + _heston_model._kappa * _heston_model._theta;
    std::complex<double> denom = (_heston_model._sigma * _heston_model._sigma) * ((a - b) * (T - time) - 2.0 * std::log((1.0 - g * std::exp(-b * (T - time))) / (1.0 - g)));
    std::complex<double> C = num / denom;

    std::complex<double> D = (a - b) / (_heston_model._sigma * _heston_model._sigma) * (1.0 - std::exp(b * (T - time)) / (1.0 - g * std::exp(b * (T - time))));

    return std::exp(C + D * _heston_model._v0 + j * omega * x);
}
