#include "ispecid.h"
#include "utils.h"
#include <map>

using namespace ispecid::datatypes;
using namespace ispecid::fileio;

int main(int argc, char **argv) {
  std::string file_path = utils::argParse<std::string>(
      argc, argv,
      "--data=", "/Users/lmpn/Desktop/diss/datasets/tsv/canidae.tsv");
  int threads = utils::argParse<int>(argc, argv, "--threads=", 1);
  std::string matrix_file_path =
      utils::argParse<std::string>(argc, argv, "--matrix=", "");
  std::vector<std::string> header;
  auto records = loadFile<record>(file_path, toRecord, Format::TSV);
  records = utils::filter(records, record::goodRecord);
  dataset data = utils::group(records, record::getSpeciesName,
                              species::addRecord, species::fromRecord);
  distance_matrix distances;
  if (matrix_file_path != "") {
    auto neigh = loadFile<neighbour>(matrix_file_path, toNeighbour, Format::CSV);
    std::transform(neigh.begin(), neigh.end(), std::inserter(distances, distances.end()), [](neighbour n){
      return std::make_pair(n.clusterA, n);
    });
  }
  grading_parameters params;
  ispecid::execution::iengine engine(threads);
  ispecid::execution::auditor auditor(params, &engine);
  auto t1 = std::chrono::high_resolution_clock::now();
  auditor.execute_with_distances(data,distances);
  // auto errors = engine.annotate(data,distances,params);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << "Duration: " << duration << std::endl;
}