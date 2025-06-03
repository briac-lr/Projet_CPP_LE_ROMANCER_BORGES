#pragma once

#include <windows.h>
#include "xlcall.h"

#include <vector>

// Convert an Excel range to std::vector<double>
std::vector<double> xlRangeToVector(const XLOPER12* inputRange); 

// Convert an Excel range to std::vector<std::vector<double>>
std::vector<std::vector<double>> xlRangeToMatrix(const XLOPER12* inputRange);

// Convert std::vector<double> to an Excel range
XLOPER12* vectorToXlRange(const std::vector<double>& vec, int rows, int cols);

// Convert std::vector<std::vector<double>> to an Excel range
XLOPER12* matrixToXlRange(const std::vector<std::vector<double>>& matrix);
