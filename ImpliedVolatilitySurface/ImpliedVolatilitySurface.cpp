#include "ImpliedVolatilitySurface.h"
#include "ThomasSolver.h"

#include <algorithm>
#include <cmath>

ImpliedVolatilitySurface::ImpliedVolatilitySurface(
	const Vector& maturities, 
	const Vector& strikes, 
	const Matrix& market_implied_vols, 
	const double& risk_free_rate)
	: _maturities(maturities), 
	_strikes(strikes), 
	_market_implied_volatilities(market_implied_vols), 
	_risk_free_rate(risk_free_rate)
{
	// sanity checks
	if (!ordered_vectors())
		throw "The maturities and strikes must be ordered and positive [strictly positive for maturities]";
	if (!market_dimensions())
		throw "The dimensions of the implied vol matrix must be (maturity dim x strike dim)";
	if (!vols_positivity())
		throw "The implied volatilities must be greater than 0.";

	// _delta_strikes[j] = K_{j+1} - K_j
	for (size_t j = 0; j < _strikes.size() - 1; ++j)
		_delta_strikes.push_back(_strikes[j + 1] - _strikes[j]);

	evaluate_cubic_spline_coefficients();
}

double ImpliedVolatilitySurface::implied_volatility(const double& maturity, const double& strike) const
{
	// This re-uses the private smile_implied_Volatility call by using the linear interpolation in variance or extrapolation

	if (maturity <= _maturities[0])
	{
		double strike_0 = strike * std::exp(_risk_free_rate * (_maturities[0] - maturity));
		return smile_implied_volatility(0, strike_0);
	}
	else if (maturity >= _maturities[_maturities.size() - 1])
	{
		double strike_M = strike * std::exp(_risk_free_rate * (_maturities[_maturities.size() - 1] - maturity));
		return smile_implied_volatility(_maturities.size() - 1, strike_M);
	}

	// Here there exists i between 1 and M-1 such as T in [T_i, T{i+1}]
	// Find the index:
	size_t lower_index = 0;
	while (maturity >= _maturities[lower_index])
		lower_index++;
	lower_index--;

	size_t upper_index = lower_index + 1;

	// Evaluate K^(i) and K^(i+1) - No need for S_0, just rate is needed, hence passed as a private data of the object.
	double left_strike = strike * std::exp(_risk_free_rate * (_maturities[lower_index] - maturity));
	double right_strike = strike * std::exp(_risk_free_rate * (_maturities[upper_index] - maturity));

	// Compute v(T_i, k_F) and v(T_{i+1}, k_F) : use of cubic spline interpolation/extrapolation
	double left_implied_vol = smile_implied_volatility(lower_index, left_strike);
	double left_total_variance = left_implied_vol * left_implied_vol * _maturities[lower_index];

	double right_implied_vol = smile_implied_volatility(upper_index, right_strike);
	double right_total_variance = right_implied_vol * right_implied_vol * _maturities[upper_index];

	// Linear interpolation in variance along maturities
	double implied_total_variance = left_total_variance + ((right_total_variance - left_total_variance) / (_maturities[upper_index] - _maturities[lower_index])) * (maturity - _maturities[lower_index]);

	double impliedVol = std::sqrt(implied_total_variance / maturity);
	return impliedVol;
}

bool ImpliedVolatilitySurface::ordered_vectors() const
{
	return std::is_sorted(_maturities.begin(), _maturities.end()) && std::is_sorted(_strikes.begin(), _strikes.end()) && _maturities[0] > 0 && _strikes[0] >= 0;
}

bool ImpliedVolatilitySurface::market_dimensions() const
{
	return _market_implied_volatilities.size() == _maturities.size() && _market_implied_volatilities[0].size() == _strikes.size();
}

bool ImpliedVolatilitySurface::vols_positivity() const
{
	for (size_t mat_idx = 0; mat_idx < _maturities.size(); ++mat_idx)
		for (size_t strike_idx = 0; strike_idx < _strikes.size(); ++strike_idx)
			if (_market_implied_volatilities[mat_idx][strike_idx] <= 0.)
				return false;

	// If the loop has finished, it means that no negative value has been found
	return true;
}

double ImpliedVolatilitySurface::smile_implied_volatility(const size_t& maturity_index, const double& strike) const
{
	double implied_vol;

	// Locate the strike in the right strike interval from _strikes : : K in [K_j, K_{j+1}] or K < K_1 or K > K_N

	// left extrapolation case
	if (strike <= _strikes[0])
	{
		// sigma(T_i, K) = sigma(T_i, K_1) + gamma_1 * (K - K_1)
		double deriv_left = _gamma_coefficients[maturity_index][0];
		implied_vol = _market_implied_volatilities[maturity_index][0] + deriv_left * (strike - _strikes[0]);
	}
	else
	{
		// right extrapolation case
		if (strike >= _strikes[_strikes.size() - 1])
		{
			// sigma(T_i, K) = sigma(T_i, K_N) + (3 alpha_{N-1} Delta^2 x_{N-1} + 2 beta_{N-1} Delta x_{N-1} + gamma_{N-1}) * (K - K_N)
			double deriv_right = 3. * _alpha_coefficients[maturity_index][_strikes.size() - 2] * _delta_strikes[_strikes.size() - 2] * _delta_strikes[_strikes.size() - 2] + 2. * _beta_coefficients[maturity_index][_strikes.size() - 2] * _delta_strikes[_strikes.size() - 2] + _gamma_coefficients[maturity_index][_strikes.size() - 2];
			implied_vol = _market_implied_volatilities[maturity_index][_strikes.size() - 1] + deriv_right * (strike - _strikes[_strikes.size() - 1]);
		}
		else
			// interpolation case	
		{
			size_t right_strike_index = 0;
			// the loop stops when K < K_{j+1} so that K_j <= K < K_{j+1} 
			while (strike >= _strikes[right_strike_index])
				right_strike_index++;

			size_t left_strike_index = right_strike_index - 1;
			// Cubic spline index is "left_strike_index"

			// Evaluate that cubic spline function at strike K
			double alpha = _alpha_coefficients[maturity_index][left_strike_index];
			double beta = _beta_coefficients[maturity_index][left_strike_index];
			double gamma = _gamma_coefficients[maturity_index][left_strike_index];

			double dx = strike - _strikes[left_strike_index];
			implied_vol = alpha * dx * dx * dx + beta * dx * dx + gamma * dx + _market_implied_volatilities[maturity_index][left_strike_index];
		}
	}

	return implied_vol;
}

void ImpliedVolatilitySurface::evaluate_cubic_spline_coefficients()
{
	// Compute _beta - Thomas Solver
	for (size_t mat_idx = 0; mat_idx < _maturities.size(); ++mat_idx)
	{
		Vector lower_diag;
		Vector central_diag;
		Vector upper_diag;
		Vector rhs;

		for (size_t j = 0; j < _strikes.size() - 3; ++j)
		{
			lower_diag.push_back(_delta_strikes[j + 1]);
			upper_diag.push_back(_delta_strikes[j + 1]);
		}

		Vector Y;
		for (size_t j = 0; j < _strikes.size(); ++j)
			Y.push_back(_market_implied_volatilities[mat_idx][j]);

		for (size_t j = 0; j < _strikes.size() - 2; ++j)
		{
			central_diag.push_back(2. * (_delta_strikes[j] + _delta_strikes[j + 1]));
			rhs.push_back(3. * ((Y[j + 2] - Y[j + 1]) / _delta_strikes[j + 1] - (Y[j + 1] - Y[j]) / _delta_strikes[j]));
		}

		ThomasSolver thomas_solver(lower_diag, central_diag, upper_diag, rhs);
		Vector solution = thomas_solver.solve();

		Vector beta; // maturity T[mat_idx]
		beta.push_back(0.);
		for (size_t j = 1; j < _strikes.size() - 1; ++j)
			beta.push_back(solution[j - 1]);
		_beta_coefficients.push_back(beta);

		Vector alpha; // maturity T[mat_idx]
		for (size_t j = 0; j < _strikes.size() - 2; ++j)
			alpha.push_back((beta[j + 1] - beta[j]) / (3. * _delta_strikes[j]));
		alpha.push_back(-beta[_strikes.size() - 2] / (3. * _delta_strikes[_strikes.size() - 2]));
		_alpha_coefficients.push_back(alpha);

		Vector gamma; // maturity T[mat_idx]
		for (size_t j = 0; j < _strikes.size() - 1; ++j)
			gamma.push_back((Y[j + 1] - Y[j]) / _delta_strikes[j] - alpha[j] * _delta_strikes[j] * _delta_strikes[j] - beta[j] * _delta_strikes[j]);
		_gamma_coefficients.push_back(gamma);
	}
}
