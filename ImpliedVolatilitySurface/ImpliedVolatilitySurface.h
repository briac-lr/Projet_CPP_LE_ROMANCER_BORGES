#ifndef IMPLIEDVOLATILITYSURFACE_H
#define IMPLIEDVOLATILITYSURFACE_H

#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

class ImpliedVolatilitySurface
{
public:
	// Constructor with parameters
	ImpliedVolatilitySurface(const Vector& maturities,
		const Vector& strikes,
		const Matrix& market_implied_vols,
		const double& risk_free_rate);

	// sigma^*( T, K)
	double implied_volatility(const double& maturity, const double& strike) const;

	inline double risk_free_rate() const
	{
		return _risk_free_rate;
	}


private:
	// sanity checks
	bool ordered_vectors() const;
	bool market_dimensions() const;
	bool vols_positivity() const;


	// private function members [methods]
	double smile_implied_volatility(const size_t& maturity_index, const double& strike) const; // sigma^*(T_i, K)
	void evaluate_cubic_spline_coefficients(); //NON-const because modifies _alpha _beta _gamma private data members

	// Caching [data members]
	Matrix _alpha_coefficients;
	Matrix _beta_coefficients;
	Matrix _gamma_coefficients;
	Vector _delta_strikes;

	// Building blocks [data members]
	Vector _maturities;  // market maturities
	Vector _strikes;     // market strikes
	Matrix _market_implied_volatilities;
	double _risk_free_rate;
};





























#endif
