#include "Matrix.h"

int main()
{
	size_t nrows = 3;
	size_t ncols = 4;
	// 1 3 5 6
	// 3 5 2 4
	// 9 8 0 2

	// Uniform initalization that uses braces [modern C++]
	std::vector<std::vector<double>> base_data = { {1., 3., 5., 6.},
										      {3., 5., 2., 4.},
										      {9., 8., 0., 2.} };

	// Classic way
	/*std::vector<std::vector<double>> data;
	std::vector<double> row1;
	row1.push_back(1.);
	row1.push_back(3.);
	row1.push_back(5.);
	row1.push_back(6.);
	data.push_back(row1);
	// ...............
	*/

	Matrix base_matrix(nrows, ncols, base_data);

	size_t dim = 3;
	// 1 3 5 
	// 3 5 2
	// 9 8 0

	// Uniform initalization that uses braces [modern C++]
	std::vector<std::vector<double>> derived_data = { {1., 3., 5.},
													  {3., 5., 2.},
											          {9., 8., 0.} };
	SquareMatrix derived_matrix(dim, derived_data);

	std::string base_name = base_matrix.call_my_name();       // "Matrix"
	std::string derived_name = derived_matrix.call_my_name(); // "SquareMatrix"

	// special case
	
	// Copy constructor of Matrix(const Matrix& other)
	// SquareMatrix other(dim, derived_data);
	// Matrix(const Matrix& other)
	// Matrix matrix = SquareMatrix(dim, derived_data);

	Matrix* matrix_ptr = new SquareMatrix(dim, derived_data);
	std::string ptr_name = matrix_ptr->call_my_name();  //(*matrix_ptr).call_my_name();
	// What is the result ?
	// If Matrix::call_my_name() is not declared virtual -> "Matrix"
	// If Matrix::call_my_name() is declared virtual -> "SquareMatrix"

	return 0;
}