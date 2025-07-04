#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// ---------------------------------------------------------------------------------------------------------------------------
// ------------------------------------ CSV READER DEFINITION ----------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------

// Copy Constructor
CSVReader::CSVReader(const std::string& path, bool has_header, char delimiter)
    : _path(path), _has_header(has_header), _delimiter(delimiter) 
{
}

// Read the file and return all numeric rows, each row is guaranteed to contain exactly 4 doubles
std::vector<std::vector<double>> CSVReader::read() const
{
    std::ifstream file(_path);
    if (!file.is_open())
        throw std::runtime_error("CSVReader: can't open \"" + _path + "\"");

    std::string line;
    std::vector<std::vector<double>> data;

    // Optional header (ignore header)
    if (_has_header && std::getline(file, line)) {}

    while (std::getline(file, line)) {
        // If there is a blank line
        if (line.empty()) continue;

        std::vector<double> row;
        std::stringstream   ss(line);
        std::string         cell;

        while (std::getline(ss, cell, _delimiter)) {
            try {
                row.emplace_back(std::stod(cell));
            }
            catch (const std::invalid_argument&) {
                throw std::runtime_error("CSVReader: non-numeric field in \"" + _path + "\" -> \"" + cell + "\"");
            }
        }
        if (row.size() != 4)
            throw std::runtime_error("CSVReader: expected 4 columns, got " +
                std::to_string(row.size()) + " in line \"" + line + '"');

        data.emplace_back(std::move(row));
    }
    return data;
}
