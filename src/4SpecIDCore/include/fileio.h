#ifndef FILEIO_H
#define FILEIO_H
#include <string>
#include <vector>
#include <functional>
STRICT_MODE_OFF
#include "csv.hpp"
STRICT_MODE_ON
#include "datatypes.h"

namespace ispecid
{
    namespace fileio
    {
        using namespace datatypes;
        enum class Format
        {
            CSV,
            TSV
        };

        template <class T>
        using OutMapper = std::function<std::vector<std::string>(T)>;
        template <class T>
        using InMapper = std::function<T(csv::CSVRow &)>;

        inline InMapper<record> toRecord =
            [](csv::CSVRow &row) {
                auto species_name = row["species_name"].get();
                auto source = row["institution_storing"].get();
                auto cluster = row["bin_uri"].get();
                record rec(species_name, cluster, source, "U");
                return rec;
            };

        inline InMapper<neighbour> toNeighbour =
            [](csv::CSVRow &row) {
                auto nA = row["neighbour_a"].get();
                auto nB = row["neighbour_b"].get();
                auto dist = std::atof(row["distance"].get().c_str());
                neighbour n{nA, nB, dist};
                return n;
            };

        template <class T>
        std::vector<T> loadFile(std::string file_path, InMapper<T> mapper, Format file_format = Format::TSV)
        {
            std::vector<T> rows;
            csv::CSVFormat format;
            if (file_format == Format::CSV)
            {
                format.delimiter(';').header_row(0);
            }
            else if (file_format == Format::TSV)
            {
                format.delimiter('\t').header_row(0);
            }
            csv::CSVReader reader(file_path, format);

            for (auto &row : reader)
            {
                T record = mapper(row);
                rows.push_back(record);
            }
            return rows;
        }

        template <class T>
        std::vector<T> loadFile(std::string file_path, std::vector<std::string> &header, InMapper<T> mapper, Format file_format = Format::TSV)
        {
            std::vector<T> rows;
            csv::CSVFormat format;
            if (file_format == Format::CSV)
            {
                format.delimiter(';').header_row(0);
            }
            else if (file_format == Format::TSV)
            {
                format.delimiter('\t').header_row(0);
            }
            csv::CSVReader reader(file_path, format);
            if (header.size() == 0)
            {
                header = reader.get_col_names();
            }

            for (auto &row : reader)
            {
                T record = mapper(header, row);
                rows.push_back(record);
            }
            return rows;
        }

        template <class T>
        void saveCsvFile(std::string file_path, std::vector<std::string> &header, std::vector<T> &rows, OutMapper<T> mapper)
        {
            std::ofstream ofs(file_path, std::ofstream::out | std::ofstream::binary);
            if (ofs)
            {
                auto writer = csv::make_csv_writer(ofs);
                writer << header;
                for (auto &row : rows)
                {
                    writer << mapper(row);
                }
                ofs.flush();
                ofs.close();
            }
        }

        template <class T = std::vector<std::string>>
        void saveTsvFile(std::string file_path, std::vector<std::string> &header, std::vector<T> &rows, OutMapper<T> mapper)
        {
            std::ofstream ofs(file_path, std::ofstream::out | std::ofstream::binary);
            if (ofs)
            {
                auto writer = csv::make_tsv_writer(ofs);
                writer << header;
                for (auto &row : rows)
                {
                    writer << mapper(row);
                }
                ofs.flush();
                ofs.close();
            }
        }

        template <class T = std::vector<std::string>>
        void saveFile(std::string file_path, std::vector<std::string> &header, std::vector<T> &rows, OutMapper<T> mapper, Format out_format = Format::TSV)
        {
            if (out_format == Format::CSV)
            {
                saveTsvFile(file_path, header, rows, mapper);
            }
            saveCsvFile(file_path, header, rows, mapper);
        }

    } // namespace fileio
} // namespace ispecid
#endif // IENGINE_H
