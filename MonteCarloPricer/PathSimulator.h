#ifndef PATHSIMULATOR_H
#define PATHSIMULATOR_H

#include <vector>
#include "RandomNormalGenerator.h"

// forward declaration
class Model;
//class StandardGaussianGenerator;

//#include "Model.h"

// Abstract class
class PathSimulator
{
public:
	PathSimulator() = default;

	// constructor with parameters
	PathSimulator(const std::vector<double> time_points, const Model& model, const double init_value);

	// Copy constructor
	PathSimulator(const PathSimulator& path_simulator);

	// assignment operator
	PathSimulator& operator=(const PathSimulator& path_simulator);

	virtual PathSimulator* clone() const = 0;

	std::vector<double> path() const;
	
	inline std::vector<double> time_points() const
	{
		return _time_points;
	}

	virtual ~PathSimulator();

protected:
	virtual double next_step(const size_t& time_index, const double& current_price) const = 0;

	std::vector<double> _time_points; // assuming it is ordered, starts with 0., etc...
	const Model* _model;
	double _initial_value;

	// Random Number Generator initialized at construction
	StandardGaussianGenerator _normal_generator;
};


class EulerPathSimulator : public PathSimulator
{
public:
	// constructor with parameters
	EulerPathSimulator(const std::vector<double> time_points, const Model& model, const double init_value);

	// Copy constructor
	EulerPathSimulator(const EulerPathSimulator& path_simulator);

	// assignment operator
	EulerPathSimulator& operator=(const EulerPathSimulator& path_simulator);

	~EulerPathSimulator() = default;

	EulerPathSimulator* clone() const override;

private:
	double next_step(const size_t& time_index, const double& current_price) const override;
};


#endif // !PATHSIMULATOR_H

