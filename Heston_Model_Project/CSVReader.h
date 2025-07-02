#pragma once
#include <string>
#include <vector>

class CSVReader {
public:
    // Copy Constructor
    CSVReader(const std::string& path, bool has_header = true, char delimiter = ',');

    // Read the file and return all numeric rows, each row is guaranteed to contain exactly 4 doubles
    std::vector<std::vector<double>> read() const;

private:
    // Absolute or relative file path
    std::string _path;
    // Skip the first line if true (header) line
    bool _has_header;
    // Field separator (default to ',')
    char _delimiter;
};
