#include "CallPrice.h"
#include <complex>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace boost::math::quadrature;

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
        u_i = -1.0;
        y_i = j * omega - 1.0;
    }
    else {
        u_i = 1.0;
        y_i = j * omega;
    }

    std::complex<double> a = _heston_model._kappa - _heston_model._rho * _heston_model._sigma* y_i;
    std::complex<double> b = std::sqrt(a * a + _heston_model._sigma * _heston_model._sigma * (u_i * j * omega + omega * omega));
    std::complex<double> g = (a - b) / (a + b);

    std::complex<double> denom = (_heston_model._sigma * _heston_model._sigma) * ((a - b) * (T - time) - 2.0 * std::log((1.0 - g * std::exp(-b * (T - time))) / (1.0 - g)));
    std::complex<double> C = (j * omega * _heston_model._r * (T - time) +
                             (_heston_model._kappa * _heston_model._theta) / (_heston_model._sigma * _heston_model._sigma) *
                             ((a - b) * (T - time) - 2.0 * std::log((1.0 - g * std::exp(-b * (T - time))) / (1.0 - g)))
                             );

    std::complex<double> D = (a - b) / (_heston_model._sigma * _heston_model._sigma) * (1.0 - std::exp(-b * (T - time))) / (1.0 - g * std::exp(-b * (T - time)));

    return std::exp(C + D * _heston_model._v0 + j * omega * x);
}


double HestonPricer::RealIntegrand(double u, double T, double time, double S0, double K, bool P1) {
    if (u == 0.0) {
        return 0.0; // Évite la division par zéro
    }

    std::complex<double> j(0.0, 1.0);

    double variable_change = u / (1 - u * u);
    double numerator = 1 + u * u;
    double denominator = std::pow(1 - u * u, 2);

    // ln(S0)
    double x = std::log(S0);

    // Phi_i
    std::complex<double> phi = CharacteristicFunction(variable_change, T, time, x, P1);

    // exp(-j * omega * ln(K))
    std::complex<double> exp_term = std::exp(-j * variable_change * std::log(K));
    
    //factor due to variable change
    double factor = numerator / denominator;

    // compute
    std::complex<double> integrand = factor * (phi * exp_term) / (j * variable_change);

    // real part
    return std::real(integrand);
}


double HestonPricer::Pi(double T, double time, double S0, double K, bool P1) {

    const int N = 64;
    gauss<double, N> integrator;

    auto integrand = [&](double u) {
        return RealIntegrand(u, T, time, S0, K, P1);
        };

    double integral = integrator.integrate(integrand, -1.0, 1.0);

    return 0.5 + (1.0 / (2.0 * M_PI)) * integral;
}

double HestonPricer::CallHestonPrice(double T, double time, double S0, double K) {

    double P1 = Pi(T, time, S0, K, true);
    double P2 = Pi(T, time, S0, K, false);

    //std::cout << "P1 = " << P1 << ", P2 = " << P2 << std::endl;

    double call_price = S0 * P1 - K * std::exp(-_heston_model._r * T) * P2;

    return call_price;
}