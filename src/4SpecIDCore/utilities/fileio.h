#ifndef FILEIO_H
#define FILEIO_H
#include <string>
#include <vector>
#include <functional>
#include "csv.h"

namespace ispecid
{
    namespace fileio
    {
        enum class Format
        {
            CSV,
            TSV
        };

        template <class Mapper, typename In = typename std::invoke_result<Mapper, csv::CSVRow &>::type>
        std::vector<In> loadFile(std::string &path, std::vector<std::string> &header, Format fmt, Mapper m)
        {
            csv::CSVFormat format;
            if (fmt == Format::CSV)
            {
                format.delimiter(';').header_row(0);
            }
            else if (fmt == Format::TSV)
            {
                format.delimiter('\t').header_row(0);
            }
            std::vector<In> rows;
            try
            {
                csv::CSVReader reader(path, format);
                header = reader.get_col_names();
                for (auto &row : reader)
                {
                    auto record = m(row);
                    rows.push_back(record);
                }
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << e.what() << '\n';
            }

            return rows;
        }

        std::vector<std::vector<std::string>>
        loadFile(std::string &path, std::vector<std::string> &header, Format format = Format::TSV)
        {
            return loadFile(path, header, format, [](auto &row) {
                return row.operator std::vector<std::string>();
            });
        }

        template <class Mapper, class T>
        void saveCsvFile(std::string path, std::vector<std::string> &header, std::vector<T> &rows, Mapper m)
        {
            std::ofstream ofs(path, std::ofstream::out | std::ofstream::binary);
            if (ofs)
            {
                auto writer = csv::make_csv_writer(ofs);
                writer << header;
                for (auto &row : rows)
                {
                    writer << m(row);
                }
                ofs.flush();
                ofs.close();
            }
        }

        template <class Mapper, class T>
        void saveTsvFile(std::string path, std::vector<std::string> &header, std::vector<T> &rows, Mapper m)
        {
            std::ofstream ofs(path, std::ofstream::out | std::ofstream::binary);
            if (ofs)
            {
                auto writer = csv::make_tsv_writer(ofs);
                writer << header;
                for (auto &row : rows)
                {
                    writer << m(row);
                }
                ofs.flush();
                ofs.close();
            }
        }

        template <class Mapper, class T>
        void saveFile(std::string path, std::vector<std::string> &header, std::vector<T> &rows, Mapper m, Format out_format = Format::TSV)
        {
            if (out_format == Format::CSV)
            {
                saveTsvFile(path, header, rows, m);
            }
            else
            {
                saveCsvFile(path, header, rows, m);
            }
        }

    } // namespace fileio
} // namespace ispecid
#endif // IENGINE_H
