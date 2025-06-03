#ifndef THOMASSOLVER_H
#define THOMASSOLVER_H

#include <vector>

using Vector = std::vector<double>;

class ThomasSolver
{
public:
	// Constructor with parameters
	ThomasSolver(const Vector& lower_diag,
		const Vector& central_diag,
		const Vector& upper_diag,
		const Vector& rhs);

	// Solution X = [X_1, ..., X_N]
	Vector solve() const;

private:
	Vector _lower_diagonal;     // [a_2, a_3, ..., a_N]
	Vector _central_diagonal;   // [b_1, b_2, ..., b_{N-1}, b_N]
	Vector _upper_diagonal;     // [c_1, c_2, ..., c_{N-1}]
	Vector _right_hand_side;    // [R_1, R_2, ..., R_{N-1}, R_N]
};














#endif // !THOMASSOLVER_H

