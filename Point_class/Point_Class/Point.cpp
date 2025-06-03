#include "Point.h"

void Point::set_abscissa(double a)
{
	_abscissa = a;
}

void Point::set_ordinate(double o)
{
	_ordinate = o;
}

double Point::get_abscissa() const
{
	return _abscissa;
}

double Point::get_ordinate() const
{
	return _ordinate;
}

double Point::static_method(double param)
{
	return 2. * param;
}

// Default constructor
Point::Point()
	: _abscissa(0.), _ordinate(0.)     // Initialization list
{
}

// Constructor with parameters
Point::Point(double abs, double ord)
	: _abscissa(abs), _ordinate(ord)  // calls the copy constructor
{
}

// Copy constructor
//Point::Point(Point point) // infinite loop of copy constructor calls
//{}

Point::Point(const Point& point)
	: _abscissa(point._abscissa), _ordinate(point._ordinate)
{}

// copy assignment operator
Point& Point::operator=(const Point& point)
{
	if (this != &point)
	{
		_abscissa = point._abscissa;
		_ordinate = point._ordinate;
	}
	return *this;
}

// destructor
Point::~Point()
{
}
