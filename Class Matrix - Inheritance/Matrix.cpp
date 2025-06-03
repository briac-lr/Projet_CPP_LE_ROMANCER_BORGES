#include "Matrix.h"

///////////////////////////////// Matrix class //////////////////////////////////////////////////

Matrix::Matrix()
	: _nrows(0), _ncols(0) , _data(std::vector<std::vector<double>>())
{
}

Matrix::Matrix(size_t nrows, size_t ncols, const std::vector<std::vector<double>>& data)
	// initialisation list
	: _nrows(nrows), _ncols(ncols), _data(data)
{
	if (!sanity_check())
		throw "Error, the sizes are not consistent!";
}

Matrix::Matrix(const Matrix& matrix)
	:_nrows(matrix._nrows), _ncols(matrix._ncols), _data(matrix._data)
{
}

Matrix& Matrix::operator=(const Matrix& matrix)
{
	if (this != &matrix)
	{
		_nrows = matrix._nrows;
		_ncols = matrix._ncols;
		_data = matrix._data;
	}
	return *this;
}

/*Matrix::~Matrix()
{
}*/

std::string Matrix::call_my_name() const
{
	return "Matrix";
}

bool Matrix::sanity_check() const
{
	// sanity check : verify that data.size() == nrows and data[i].size() == ncols for all i in [[0,..., nrows-1]]
	// TO DO ...
	return true;
}


//////////////////////////////////////// SquareMatrix class /////////////////////////////////////////////////


SquareMatrix::SquareMatrix()
	: Matrix(), _trace(0), _determinant(0)
{
}

SquareMatrix::SquareMatrix(size_t dim, const std::vector<std::vector<double>>& data)
	: Matrix(dim, dim, data), _trace(trace()), _determinant(determinant())
{
}

SquareMatrix::SquareMatrix(const SquareMatrix& matrix)
	: Matrix(matrix), _trace(matrix._trace), _determinant(matrix._determinant)
{
}

SquareMatrix& SquareMatrix::operator=(const SquareMatrix& matrix)
{
	if (this != &matrix)
	{
		Matrix::operator=(matrix);
		_trace = matrix._trace;
		_determinant = matrix._determinant;
	}
	return *this;
}


std::string SquareMatrix::call_my_name() const
{
	return "SquareMatrix";
}

double SquareMatrix::trace()
{
	double trace = 0.;
	for (size_t idx = 0; idx < _nrows; idx++)
		trace += _data[idx][idx];
	return trace;
}

double SquareMatrix::determinant()
{
	// TO DO
	return 0.0;
}
