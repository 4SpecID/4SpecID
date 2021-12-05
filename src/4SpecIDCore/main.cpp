#include <representations/record.h>
#include <representations/species.h>
#include <representations/gradingParameters.h>
#include <representations/distance.h>
#include <representations/types.h>
#include <network/network.h>
#include <auditor/auditor.h>
#include "utils.h"
#include "engine.h"
#include "fileio.h"
#include <map>
#include <optional>
#include <functional>
#include <algorithm>
#include <boost/regex.hpp>
using namespace ispecid;

int main(int argc, char **argv)
{
  auto help = utils::argParse(argc, argv, "--help=", false);
  auto file_path = utils::argParse(argc, argv, "--data=", std::string(""));
  auto matrix_file_path = utils::argParse(argc, argv, "--matrix=", std::string(""));
  auto threads = utils::argParse(argc, argv, "--threads=", 1);
  auto size = utils::argParse(argc, argv, "--size=", 3);
  auto sources = utils::argParse(argc, argv, "--sources=", 2);
  auto distance = utils::argParse(argc, argv, "--distance=", 2.0);
  auto out_distance = utils::argParse(argc, argv, "--out_distance=", std::string("distances.tsv"));
  auto out_data = utils::argParse(argc, argv, "--auto_data=", std::string("data.tsv"));

  if (help || file_path == "")
  {
    std::cout << "Usage: 4specid"
                 "\n\t--data=<path_to_dataset>"
                 "\n\t--help=<true>(optional)"
                 "\n\t--matrix=<number_of_threads>(optional)"
                 "\n\t--threads=<path_to_matrix>(optional)"
                 "\n\t--size=<path_to_distance_matrix>(optional)"
                 "\n\t--sources=<path_to_distance_matrix>(optional)"
                 "\n\t--distance=<path_to_distance_matrix>(optional)\n";
    return 1;
  }

  std::string cluster = "BOLD:AAC5230";
  std::string host = "http://v4.boldsystems.org/index.php/Public_BarcodeCluster?clusteruri=";
  network::HttpEngine he(host);

  // auto toRecord = [](csv::CSVRow &row) {
  //               auto species_name = row["species_name"].get();
  //               auto source = row["institution_storing"].get();
  //               auto cluster = row["bin_uri"].get();
  //               Record rec(species_name, cluster, source, "U");
  //               return rec;
  //           };
  

  auto t1 = std::chrono::high_resolution_clock::now();
  auto toRecord = [](csv::CSVRow &row) {
                auto species_name = row["species_name"].get();
                auto source = row["institution_storing"].get();
                auto cluster = row["bin_uri"].get();
                representations::Record rec(species_name, cluster, source, "U", 1);
                return rec;
            };
  auto toDistance = [](csv::CSVRow &row) {
      auto nA = row["neighbour_a"].get();
      auto nB = row["neighbour_b"].get();
      auto dist = std::atof(row["distance"].get().c_str());
      representations::Distance n = representations::Distance(nA, nB, dist);
      return n;
  };
  std::vector<std::string> h;
  auto records = fileio::loadFile(file_path, h, fileio::Format::TSV, toRecord);
  std::cout << records.size();
  representations::DistanceMatrix distances;
  if (matrix_file_path != "") {
    auto neigh = loadFile(matrix_file_path, h, fileio::Format::TSV, toDistance);
    std::transform(neigh.begin(), neigh.end(), std::inserter(distances, distances.end()), [](representations::Distance n){
      return std::make_pair(n.m_clusterA, n);
    });
  }
  records = utils::filter(records,  representations::Record::goodRecord);
  auto data = utils::group(records, representations::Record::getSpeciesName,
                               representations::Species::addRecord);
  representations::GradingParameters params;
  execution::Engine* engine = new execution::Engine(threads);
  execution::Auditor aud(params, engine);
  aud.executeWithDistances(data,distances);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << "Duration: " << duration << std::endl;
  return 0;
}