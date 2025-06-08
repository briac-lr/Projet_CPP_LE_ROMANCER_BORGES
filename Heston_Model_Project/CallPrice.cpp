#include "CallPrice.h"

#include <complex>
#include <cmath>
#include <functional>
#include <numbers>

const double pi = std::numbers::pi;

HestonPricer::HestonPricer(HestonModel model_) : model(model_) 
{
}

double HestonPricer::calculatePrice(double S0, double K, double T) {
    auto integrand = [&](double omega, bool P1) {
        std::complex<double> j(0.0, 1.0);
        std::complex<double> f = model.characteristicFunction(omega, T, std::log(S0), P1);
        std::complex<double> numerator = f * std::exp(-j * omega * std::log(K));
        std::complex<double> denominator = j * omega;
        return std::real(numerator / denominator);
        };

    auto simpson_integral = [&](bool P1) {
        const int N = 1000;
        double a = 1e-6, b = 100.0;
        double h = (b - a) / N;
        double sum = integrand(a, P1) + integrand(b, P1);
        for (int i = 1; i < N; i += 2)
            sum += 4 * integrand(a + i * h, P1);
        for (int i = 2; i < N; i += 2)
            sum += 2 * integrand(a + i * h, P1);
        return 0.5 + h / 3.0 / pi * sum;
        };

    double P1 = simpson_integral(true);
    double P2 = simpson_integral(false);
    return S0 * P1 - K * std::exp(-model.r * T) * P2;
}

