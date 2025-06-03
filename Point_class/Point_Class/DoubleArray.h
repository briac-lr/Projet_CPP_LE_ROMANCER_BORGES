//#pragma once

#ifndef DOUBLEARRAY_H
#define DOUBLEARRAY_H

// Designs a class that looks like std::vector<double>

class DoubleArray
{
public:
	DoubleArray(size_t size);
	DoubleArray(const DoubleArray& other);
	DoubleArray& operator=(const DoubleArray& other);
	~DoubleArray();

	double& operator[](size_t i);
	const double& operator[](size_t i) const;

private:
	double* _data;
	size_t _size;
};
















#endif // !DOUBLEARRAY_H
