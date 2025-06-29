#include "CSVReader.h"

#include <stdexcept>

namespace csv {

    Reader::Reader(const std::string& path, char sep)
        : sep_{ sep }, file_{ path }
    {
        if (!file_.is_open())
            throw std::runtime_error("Impossible d’ouvrir le fichier : " + path);

        std::string line;
        if (!std::getline(file_, line))
            throw std::runtime_error("Fichier CSV vide : " + path);

        if (!parseLine(line, header_))
            throw std::runtime_error("Erreur de parsing de l’en-tête : " + path);
    }

    bool Reader::readRow(std::vector<std::string>& out)
    {
        std::string line;
        if (!std::getline(file_, line))
            return false;                      // EOF ou erreur flux

        return parseLine(line, out);
    }

    bool Reader::parseLine(const std::string& line, std::vector<std::string>& out) const
    {
        out.clear();
        std::string field;
        bool inQuotes = false;

        for (std::size_t i = 0; i < line.size(); ++i) {
            char c = line[i];

            if (c == '"') {
                // Deux guillemets consécutifs -> caractère " littéral
                if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;                       // consomme le second "
                }
                else {
                    inQuotes = !inQuotes;
                }
            }
            else if (c == sep_ && !inQuotes) {
                out.push_back(field);
                field.clear();
            }
            else {
                field += c;
            }
        }
        out.push_back(field);                  // dernier champ
        return true;
    }

} // namespace csv
