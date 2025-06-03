// #pragma once

#ifndef POINT_H
#define POINT_H

class Point
{
public:
	// 1) default constructor
	Point();

	// constructor with parameters -> not a special method
	Point(double abs, double ord);

	// 2) copy constructor
	Point(const Point& point);

	// 3) copy assignment operator
	Point& operator=(const Point& point);

	// 4) Destructor
	~Point();
	
	// setter methods
	void set_abscissa(double a);
	void set_ordinate(double o);

	// getter methods
	double get_abscissa() const;
	double get_ordinate() const;


	static double static_method(double param);

private:
	// data members - attributes
	double _abscissa;
	double _ordinate;
};







#endif 
