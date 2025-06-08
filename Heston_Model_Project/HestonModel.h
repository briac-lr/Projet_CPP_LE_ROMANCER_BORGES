#ifndef HESTON_MODEL_H
#define HESTON_MODEL_H

#include <complex>

class HestonModel {
public:

    // Parameterized Constructor
    HestonModel(const double &kappa, const double &theta, const double &sigma, const double &rho, const double &v0, double r);

    // Copy Constructor
    HestonModel(const HestonModel& model);

    // Copy Assignment Operator
    HestonModel& operator=(const HestonModel& model);

    // Destructor
    ~HestonModel() = default;

    std::complex<double> characteristicFunction(
        double omega, double tau, double x, bool P1);

private:
    double _kappa, _theta, _sigma, _rho, _v0, _r;
};

#endif
