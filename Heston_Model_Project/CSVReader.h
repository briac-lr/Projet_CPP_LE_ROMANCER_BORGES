#pragma once

#include <fstream>
#include <string>
#include <vector>

/**
 * Petit utilitaire minimaliste pour lire un fichier CSV :
 *   • La 1ʳᵉ ligne est traitée comme en-tête (noms de colonnes)
 *   • Les champs peuvent être entourés de "guillemets"
 *   • Le séparateur par défaut est ‘,’ (modifiable)
 *
 * Usage :
 *   csv::Reader reader("donnees.csv");          // ouverture + lecture de l’en-tête
 *   std::vector<std::string> row;
 *   while (reader.readRow(row)) { … }           // parcours ligne à ligne
 */
namespace csv {

    class Reader {
    public:
        explicit Reader(const std::string& path, char sep = ',');
        ~Reader() = default;

        /** Renvoie les noms de colonnes lus dans l’en-tête */
        const std::vector<std::string>& header() const noexcept { return header_; }

        /**
         * Lit la ligne suivante.
         * @param out  vecteur de sortie (remplacé) qui contiendra les champs.
         * @return     false si fin de fichier ou erreur, true sinon.
         */
        bool readRow(std::vector<std::string>& out);

    private:
        bool parseLine(const std::string& line, std::vector<std::string>& out) const;

        char sep_;
        std::ifstream file_;
        std::vector<std::string> header_;
    };

} // namespace csv
