#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <vector>
#include <string>

class Calibration{
public:
    // Parameterized constructor
    Calibration(const std::array<double, 5>& initial_guess, int iterations, const char* market_data_path);

    // Copy constructor
    Calibration(const Calibration& other);

    // Copy assignment operator
    Calibration& operator=(const Calibration& other);

    // Destructor
    ~Calibration();

    // Public method to run the calibration
    void Calibrate();

    // Accessors for calibrated parameters
    double getRho() const;
    double getKappa() const;
    double getTheta() const;
    double getV0() const;
    double getSigma() const;

private:
    std::array<double, 5> _initial_guess;
    int _number_iterations;
    const char* _market_data_path;

    double _rho, _kappa, _theta, _v0, _sigma;

    //std::vector<std::vector<double>> get_data(const char* path);
};

#endif // OPTIMISATION_HPP
