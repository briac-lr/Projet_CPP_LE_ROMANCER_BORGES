#include "DoubleArray.h"

DoubleArray::DoubleArray(size_t size)
	: _data(new double[size]), _size(size)
{
}

DoubleArray::DoubleArray(const DoubleArray& other)
	// :  _data(other._data) - NO WAY - 2 objects cannot point to the same memory space
	: _data(new double[other._size]), _size(other._size)
{
	for (size_t i = 0; i < _size; ++i)
		_data[i] = other._data[i];
}

DoubleArray& DoubleArray::operator=(const DoubleArray& other)
{
	if (this != &other)
	{
		// _data already points to an array of values
		delete[] _data;  // cleans the memory of the current array _data points to
		_size = other._size;
		_data = new double[_size];
		for (size_t i = 0; i < _size; ++i)
			_data[i] = other._data[i];
	}
	return *this;
}

DoubleArray::~DoubleArray()
{
	delete[] _data;
}

double& DoubleArray::operator[](size_t i)
{
	return _data[i];
}

const double& DoubleArray::operator[](size_t i) const
{
	return _data[i];
}
