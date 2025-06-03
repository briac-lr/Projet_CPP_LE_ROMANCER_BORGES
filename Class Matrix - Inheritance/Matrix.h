#pragma once

#include <vector>
#include <string>

// Base class

class Matrix
{
public:
	// default constructor
	// Matrix() = delete;
	Matrix();

	// constructor with parameters
	Matrix(size_t nrows, size_t ncols, const std::vector<std::vector<double>>& data);

	// copy constructor
	Matrix(const Matrix& matrix);

	// copy assignement operator
	Matrix& operator=(const Matrix& matrix);

	// destructor - always virtual for base class
	virtual ~Matrix() = default;

	// method that provides the name of the class
	virtual std::string call_my_name() const;

protected:
	bool sanity_check() const;
	// data members
	size_t _nrows;
	size_t _ncols;
	std::vector<std::vector<double>> _data;
};

// Special case : square matrix

class SquareMatrix : public Matrix   // all data members of Matrix are accessible to SquareMatrix
{
public:
	// Default constructor
	SquareMatrix();

	// Constructor with parameters
	SquareMatrix(size_t dim, const std::vector<std::vector<double>>& data);

	// Copy constructor
	SquareMatrix(const SquareMatrix& matrix);

	// Copy assignment operator
	SquareMatrix& operator=(const SquareMatrix& matrix);

	// destructor
	~SquareMatrix() = default;

	// method that provides the name of the class
	std::string call_my_name() const;

private:
	double trace();
	double determinant();

	/* 
	size_t _nrows;
	size_t _ncols;
	std::vector<std::vector<double>> _data;
	*/
	double _trace;
	double _determinant;
};