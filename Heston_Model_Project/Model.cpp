#include "HestonModel.h"
#include <cmath>


// Parameterized constructor
HestonModel::HestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, double r)
    : _kappa(kappa), _theta(theta), _sigma(sigma), _rho(rho), _v0(v0), _r(r) 
{
}

// Copy constructor
HestonModel::HestonModel(const HestonModel& model)
    : _kappa(model._kappa), _theta(model._theta), _sigma(model._sigma), _rho(model._rho), _v0(model._v0), _r(model._r)
{
}

// Copy assignment operator
HestonModel& HestonModel::operator=(const HestonModel& model) {
    if (this != &model) {
        _kappa = model._kappa;
        _theta = model._theta;
        _sigma = model._sigma;
        _rho = model._rho;
        _v0 = model._v0;
        _r = model._r;
    }
    return *this;
}

// Destructor
HestonModel::~HestonModel()
{
}


// Characteristic function 
std::complex<double> HestonModel::characteristicFunction(
    double omega, double tau, double x, bool P1) {

    std::complex<double> j(0.0, 1.0);
    double u_i = P1 ? 1 : -1;

    std::complex<double> y_i = j * omega - (P1 ? 1 : 0); // j(-(P1 ? 1 : 0), omega)   


    std::complex<double> a = _kappa - _rho * _sigma * y_i;
    std::complex<double> b = std::sqrt(a * a + _sigma * _sigma * (u_i * j * omega + omega * omega));


    std::complex<double> g = (a - b) / (a + b); 

    std::complex<double> C = _r * j * omega * tau + _kappa * _theta / (_sigma * _sigma) * ((a - b) * tau 
                            - 2.0 * std::log((1.0 - g * std::exp(- b * tau)) / (1.0 - g)));

        
    std::complex<double> D = (a - b) / (_sigma * _sigma) *
                             ((1.0 - std::exp(b * tau)) / (1.0 - g * std::exp(b * tau))); //

    return std::exp(C + D * _v0 + j * omega * x);
}
