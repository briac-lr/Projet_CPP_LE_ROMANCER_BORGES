#include <string>

// variables and types

void test_native_types()
{
	// integer [Z] [-Max, +Max]
	int i = -5;
	int j;

	// size_t - [N] [0, +Max]
	size_t sz = 4;

	// float : floating type of real number - we WILL NEVER USE IT
	float f = 5.34978;

	// double : double precision 
	double d = -6.367827648;

	// character
	char c = 'c';

	// boolean
	bool b = false; // or true

	// not really a native type
	std::string s = "hello";
}


// Arrays of one specific type
void test_arrays()
{
	// create an array of 10 integers
	int arr[10];
	// first element index is always 0 in C/C++
	// arr[0] = 3;
	// arr[0] = 2;
	// .........


	// ++j, j++ ??
	int j = 3;
	int x = j++; // x = 3 -> j = 4
	int y = ++j; // j = 5 -> y = 5

	// arr[0] = 0; arr[1] = 1; ....
	for (size_t i = 0; i < 10; ++i)
		arr[i] = (int)i;
}

void test_pointers()
{
	// pointer : variable whose value is an address on another variable and contains the size of the variable

	int i = -2;
	int* p = &i;

	// dereferencing -> variable pointed by p
	*p = 3;
	// i = 3 after this operation

	i = 4; //  print(*p) -> returns 4
}

void test_stack_heap()
{
	int stackVar = 5; // static creation of variable -> stack

	int* heapPtr = new int;
	*heapPtr = 4;
	delete heapPtr; // frees the memory space of the variable pointed by the pointer
}

void array_pointer_connection()
{
	// an array can be seen as a pointer on its first element

	int arr[10];
	int* ptr = arr;

	// *ptr ??  arr[0]
	// *(ptr + 1) arr[1]
}

// Function
// Contains:
// 1) parameters
// 2) return type
// 3) name 

double sum(const double& a, const double& b)
{
	//a += 100.0; // a = a + 100
	//b -= 50.0;
	return a + b;
}

void swap(double& a, double& b)
{
	//a, b = b, a;
	double temp = a;
	a = b;
	b = temp;
}

void test_passing_by_reference_or_value()
{
	double a = 5.34;
	double b = 7.18;

	swap(a, b); // only works if parameters of swap are passed by REFERENCE
	// a? b?

	double s = sum(a, b);
}

// const
void test_const()
{
	const int v = 5;
	const int* ptr_v = &v;

	int w = 2;
	int x = 7;
	int* const ptr_w = &w;
	// ptr_w = &x; POINTER is CONST
	*ptr_w = 67;

	int* ptr = new int;
	*ptr = 4;
	ptr = &w;

	const int* ptr_to_c;
	ptr_to_c = &v;

	int* const cptr = &x;
	*cptr = 10;

	const int* const cptr_to_c = &v;
}

struct BankAccount
{
	std::string _name;
	std::string _currency;
	double _amount;

	// void display_bank_account_information();
};

void test_struct()
{
	BankAccount bank_account;
	bank_account._name = "Benoit Vandevelde";
	bank_account._currency = "GBP";
	bank_account._amount = 127.45;
}

int main()
{
	return 0;
}


