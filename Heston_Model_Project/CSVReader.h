#pragma once
#include <string>
#include <vector>

class CSVReader {
public:
    /// \param path          Absolute or relative file path
    /// \param has_header    Skip the first line if true
    /// \param delimiter     Field separator (defaults to ',')
    explicit CSVReader(const std::string& path,
        bool has_header = true,
        char delimiter = ',');

    /// Read the file and return all numeric rows.
    /// Each row is guaranteed to contain exactly 4 doubles.
    std::vector<std::vector<double>> read() const;

private:
    std::string _path;
    bool        _has_header;
    char        _delimiter;
};
