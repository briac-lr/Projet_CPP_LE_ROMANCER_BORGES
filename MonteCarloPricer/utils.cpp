#include "utils.h"

// Convert an Excel range to std::vector<double>
std::vector<double> xlRangeToVector(const XLOPER12* inputRange) {
    std::vector<double> result;

    if (inputRange->xltype != xltypeMulti) {
        return result;  // Return empty vector if input is not a range
    }

    int rows = inputRange->val.array.rows;
    int cols = inputRange->val.array.columns;

    result.reserve(rows * cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            XLOPER12* cell = &inputRange->val.array.lparray[i * cols + j];
            if (cell->xltype == xltypeNum) {
                result.push_back(cell->val.num);
            }
            else {
                result.push_back(0.0);  // Handle non-numeric cells as 0.0, or use error handling
            }
        }
    }

    return result;
}

std::vector<std::vector<double>> xlRangeToMatrix(const XLOPER12* inputRange)
{
	std::vector<std::vector<double>> result;

	if (inputRange->xltype != xltypeMulti) {
		return result;  // Return empty vector if input is not a range
	}

	int rows = inputRange->val.array.rows;
	int cols = inputRange->val.array.columns;

	result.reserve(rows);

	for (int i = 0; i < rows; ++i) {
		std::vector<double> row;
		for (int j = 0; j < cols; ++j) {
			XLOPER12* cell = &inputRange->val.array.lparray[i * cols + j];
			if (cell->xltype == xltypeNum) {
				row.push_back(cell->val.num);
			}
			else {
				row.push_back(0.0);  // Handle non-numeric cells as 0.0, or use error handling
			}
		}
		result.push_back(row);
	}

	return result;
}

// Convert std::vector<double> to an Excel range
XLOPER12* vectorToXlRange(const std::vector<double>& vec, int rows, int cols) {
    static XLOPER12 result;
    result.xltype = xltypeMulti;
    result.val.array.rows = rows;
    result.val.array.columns = cols;
    result.val.array.lparray = new XLOPER12[rows * cols];

    for (int i = 0; i < rows * cols; ++i) {
        result.val.array.lparray[i].xltype = xltypeNum;
        result.val.array.lparray[i].val.num = vec[i];
    }

    return &result;
}

XLOPER12* matrixToXlRange(const std::vector<std::vector<double>>& matrix)
{
	static XLOPER12 result;
	result.xltype = xltypeMulti;
	result.val.array.rows = matrix.size();
	result.val.array.columns = matrix[0].size();
	result.val.array.lparray = new XLOPER12[matrix.size() * matrix[0].size()];

	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix[0].size(); ++j) {
			result.val.array.lparray[i * matrix[0].size() + j].xltype = xltypeNum;
			result.val.array.lparray[i * matrix[0].size() + j].val.num = matrix[i][j];
		}
	}

	return &result;
}


