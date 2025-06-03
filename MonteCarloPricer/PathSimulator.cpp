#include "PathSimulator.h"
#include "Model.h"
#include "RandomNormalGenerator.h"


PathSimulator::PathSimulator(const std::vector<double> time_points, const Model& model, const double init_value)
	: _time_points(time_points), _model(model.clone()), _initial_value(init_value)
{
}

PathSimulator::PathSimulator(const PathSimulator& path_simulator)
	: _time_points(path_simulator._time_points), _model(path_simulator._model->clone()), _initial_value(path_simulator._initial_value)
{
}

PathSimulator& PathSimulator::operator=(const PathSimulator& path_simulator)
{
	if (this != &path_simulator)
	{
		_time_points = path_simulator._time_points;
		_model = path_simulator._model->clone();
		_initial_value = path_simulator._initial_value;
	}

	return *this;
}

std::vector<double> PathSimulator::path() const
{
	//std::vector<double> path; // 0 elements
	std::vector<double> path(_time_points.size()); // Same number of elements as _time_points

	//path.push_back(_initial_value);
	path[0] = _initial_value;

	//for (size_t idx = 1; idx < _time_points.size(); idx++)
	//	path.push_back(next_step(idx - 1, path[idx - 1]));

	for (size_t idx = 1; idx < _time_points.size(); idx++)
		path[idx] = next_step(idx - 1, path[idx - 1]);

	return path;
}

PathSimulator::~PathSimulator()
{
	delete _model;
}

EulerPathSimulator::EulerPathSimulator(const std::vector<double> time_points, const Model& model, const double init_value)
	: PathSimulator(time_points, model, init_value)
{
}

EulerPathSimulator::EulerPathSimulator(const EulerPathSimulator& path_simulator)
	: PathSimulator(path_simulator)
{
}

EulerPathSimulator& EulerPathSimulator::operator=(const EulerPathSimulator& path_simulator)
{
	if (this != &path_simulator)
		PathSimulator::operator=(path_simulator);

	return *this;
}

EulerPathSimulator* EulerPathSimulator::clone() const
{
	return new EulerPathSimulator(*this);
}

double EulerPathSimulator::next_step(const size_t& time_index, const double& current_price) const
{
	// S_{t_{i+1}} = S_{t_i} + mu(t_i, S_{t_i}) * Delta_t + sigma(t_i, S_{t_i}) Delta_W
	// Delta_W = \sqrt(Delta_t) * Epsilon 
	// Epsilon ~ N(0,1) --> we need to simulate N(0,1)

	double delta_t = _time_points[time_index + 1] - _time_points[time_index];
	double gaussian = _normal_generator.generate();
	double next_step = current_price
		+ _model->drift_term(_time_points[time_index], current_price) * delta_t
		+ _model->diffusion_term(_time_points[time_index], current_price) * sqrt(delta_t) * gaussian;

	return next_step;
}
