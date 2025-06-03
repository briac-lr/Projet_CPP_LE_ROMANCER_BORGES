
#include <windows.h>
#include "utils.h"
#include "xlcall.h"
#include "framework.h"
#include "xlOper.h"

#include "ImpliedVolatilitySurface.h"
#include "Model.h" 
#include "PathSimulator.h"
#include "Payoff.h"
#include "MonteCarlo.h"
#include "RandomNormalGenerator.h"


//	Wrappers

extern "C" __declspec(dllexport)
double xMultiply2Numbers(double x, double y)
{
	return x * y;
}


// Vector to Excel range
// Example function that takes a range, processes it, and returns a new range
extern "C" __declspec(dllexport) XLOPER12* xVectorFunction(XLOPER12* inputRange) {
	// Convert the Excel range to std::vector<double>
	std::vector<double> vec = xlRangeToVector(inputRange);

	// Process the vector (for example, multiply each element by 2)
	for (double& val : vec) {
		val *= 2.0;
	}

	// Convert the std::vector<double> back to an Excel range
	int rows = inputRange->val.array.rows;
	int cols = inputRange->val.array.columns;
	return vectorToXlRange(vec, rows, cols);
}

extern "C" __declspec(dllexport) XLOPER12* xImpliedVolatility(
	XLOPER12* market_maturities_range,
	XLOPER12* market_strikes_range,
	XLOPER12* market_implied_volatilities_range,
	XLOPER12* custom_maturities_range,
	XLOPER12* custom_strikes_range,
	double risk_free_rate
)
{
	std::vector<double> market_maturities = xlRangeToVector(market_maturities_range);
	std::vector<double> market_strikes = xlRangeToVector(market_strikes_range);
	std::vector<std::vector<double>> market_implied_volatilities = xlRangeToMatrix(market_implied_volatilities_range);
	std::vector<double> custom_maturities = xlRangeToVector(custom_maturities_range);
	std::vector<double> custom_strikes = xlRangeToVector(custom_strikes_range);

	std::vector<std::vector<double>> custom_implied_volatilities;

	ImpliedVolatilitySurface implied_volatility_surface(market_maturities, market_strikes, market_implied_volatilities, risk_free_rate);
	for (size_t row_idx = 0; row_idx < custom_maturities.size(); row_idx++)
	{
		std::vector<double> smile_vector;
		for (size_t col_idx = 0; col_idx < custom_strikes.size(); col_idx++)
			smile_vector.push_back(implied_volatility_surface.implied_volatility(custom_maturities[row_idx], custom_strikes[col_idx]));
		custom_implied_volatilities.push_back(smile_vector);
	}

	return matrixToXlRange(custom_implied_volatilities);
}

extern "C" _declspec(dllexport) XLOPER12* xEuropeanOptionMCvsAnalytics(
	double risk_free_rate,
	double initial_spot,
	double volatility,
	double maturity,
	double strike,
	int option_type, 
	int number_time_points,
	int number_simulations
)
{
	// Model
	BlackScholesModel bs_model(risk_free_rate, volatility);

	// Time Discretisation [0 ,..., T]
	std::vector<double> time_points(number_time_points + 1);
	time_points[0] = 0.;
	for (size_t idx = 1; idx < number_time_points; idx++)
		time_points[idx] = (double)idx * maturity / (double)number_time_points;
	time_points[number_time_points] = maturity;

	// Path Simulator
	EulerPathSimulator euler_path_sim(time_points, bs_model, initial_spot);

	// Payoff
	Call_Put call_put;
	/*if (option_type == "CALL")
		call_put = Call_Put::Call;
	else if (option_type == "PUT")
		call_put = Call_Put::Put;
	else
		throw "Only CALL and PUT are possible values for the option type";*/

	if (option_type == 1)
		call_put = Call_Put::Call;
	else if (option_type == -1)
		call_put = Call_Put::Put;
	else
		throw "Only 1 [Call] and -1[Put] are possible values for the option type";

	EuropeanOptionPayoff euro_option_payoff(risk_free_rate, strike, call_put);

	// MonteCarlo
	MonteCarlo monte_carlo(number_simulations, euler_path_sim, euro_option_payoff);

	// Price MC vs Anaytic
	std::vector<double> mc_vs_analytic_prices;
	// MC price
	mc_vs_analytic_prices.push_back(monte_carlo.price());
	// Analytical price
	double mult = option_type == 1 ? 1. : -1.;
	double d1 = (std::log(initial_spot / strike) + (risk_free_rate + 0.5 * volatility * volatility) * maturity) / (volatility * std::sqrt(maturity));
	double d2 = (std::log(initial_spot / strike) + (risk_free_rate - 0.5 * volatility * volatility) * maturity) / (volatility * std::sqrt(maturity));
	double nd1 = StandardGaussianGenerator::cdf(mult*d1);
	double nd2 = StandardGaussianGenerator::cdf(mult*d2);

	double analytical_price = mult * (initial_spot * nd1 - strike * std::exp(-risk_free_rate*maturity) * nd2);
	mc_vs_analytic_prices.push_back(analytical_price);

	return vectorToXlRange(mc_vs_analytic_prices, 1, 2);
}


//	Registers

extern "C" __declspec(dllexport) int xlAutoOpen(void)
{
	XLOPER12 xDLL;	

	Excel12f(xlGetName, &xDLL, 0);

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMultiply2Numbers"),
		(LPXLOPER12)TempStr12(L"BBB"),
		(LPXLOPER12)TempStr12(L"xMultiply2Numbers"),
		(LPXLOPER12)TempStr12(L"x, y"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"GustaveEiffelFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Multiplies 2 numbers"),
		(LPXLOPER12)TempStr12(L""));


	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xVectorFunction"),
		(LPXLOPER12)TempStr12(L"QQ"),
		(LPXLOPER12)TempStr12(L"xVectorFunction"),
		(LPXLOPER12)TempStr12(L"inputRange"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"GustaveEiffelFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Multiplies each element of the input range by 2"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xImpliedVolatility"),
		(LPXLOPER12)TempStr12(L"QQQQQQB"),
		(LPXLOPER12)TempStr12(L"xImpliedVolatility"),
		(LPXLOPER12)TempStr12(L"market_maturities_range, market_strikes_range, market_implied_volatilities_range, custom_maturities_range, custom_strikes_range, risk_free_rate"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"GustaveEiffelFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Interpolates/extrapolates the implied volatility surface for any maturity and any strike."),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xEuropeanOptionMCvsAnalytics"),
		(LPXLOPER12)TempStr12(L"QBBBBBIII"),
		(LPXLOPER12)TempStr12(L"xEuropeanOptionMCvsAnalytics"),
		(LPXLOPER12)TempStr12(L"risk_free_rate, initial_spot, volatility, maturity, strike, option_type, number_time_points, number_simulations"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"GustaveEiffelFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Computes the European option price by Monte Carlo and Analytical formula."),
		(LPXLOPER12)TempStr12(L""));


	/* Free the XLL filename */
	Excel12f(xlFree, 0, 1, (LPXLOPER12)&xDLL);

	return 1;
}

