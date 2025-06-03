#include "Point.h" 
#include "DoubleArray.h"


int main()
{
	Point p; // calls the default constructor
	double abscissa = p.get_abscissa();
	//p._abscissa = 4.;
	p.set_abscissa(4.);

	Point q(3., -5.);
	double static_result = Point::static_method(1.);

	Point r(2.,0.);

	(p = (q = r)); // q = r must return the reference to q
	// p.operator=(q);

	Point* ptr = new Point(r);
	delete ptr;


	DoubleArray da(3);
	da[0] = 3.5; // calls double& operator[](size_t i);
	da[1] = -2.5; // calls double& operator[](size_t i);
	da[2] = 4.7; // calls double& operator[](size_t i);

	DoubleArray db(da);

	double d = db[2]; // calls const double& operator[](size_t i) const;
	d = 5.;

	return 0;
}