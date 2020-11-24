#include "auditor.h"
#include <limits>
namespace ispecid {
namespace execution {

bool auditor::is_shared_bin(datatypes::dataset &data, const std::string &bin) {
  std::unordered_set<std::string_view> unique_set;
  size_t count = 0;
  for (auto &entry : data) {
    auto lst = entry.second.getClusters().end();
    auto result = entry.second.getClusters().find(bin);
    if (lst != result && (++count) == 2) {
      return false;
    }
  }
  return true;
}

void auditor::parse_bold_data(std::string cluster) {
  datatypes::neighbour neighbour;
  neighbour.distance = DBL_MAX;
  neighbour.clusterA = cluster;
  neighbour.clusterB = "";
  std::string url(
      "http://v4.boldsystems.org/index.php/Public_BarcodeCluster?clusteruri=" +
      std::string(cluster));
  std::string page = network::get_page(url.c_str());
  static const boost::regex distance_regex(
      "Distance to Nearest Neighbor:</th>\\s*<td>(\\d+.\\d+)%.*</td>");
  static const boost::regex cluster_regex(
      "Nearest BIN URI:</th>\\s*<td>(.*?)</td>");
  boost::smatch char_matches;
  bool no_distance, no_name;
  no_distance = no_name = false;
  if (boost::regex_search(page, char_matches, distance_regex)) {
    neighbour.distance = std::stod(char_matches[1]);
  } else {
    neighbour.distance = std::numeric_limits<float>::max();
    no_distance = true;
  }
  if (boost::regex_search(page, char_matches, cluster_regex)) {
    neighbour.clusterB = char_matches[1];
  } else {
    neighbour.clusterB = "UNDEFINED";
    no_name = true;
  }

  _matrix.insert({cluster, neighbour});

  if (no_name)
    throw std::runtime_error("neighbour not found when retrieving " + cluster);
  else if (no_distance)
    throw std::runtime_error("Distance not found when retrieving " + cluster);
}

std::vector<datatypes::neighbour>
auditor::get_neighbours(const std::unordered_set<std::string> &clusters) {
  std::vector<datatypes::neighbour> neighbours;
  auto distances_end = _matrix.end();
  for (auto &cluster : clusters) {
    auto neighbour_it = _matrix.find(cluster);
    if (neighbour_it != distances_end) {
      neighbours.push_back((*neighbour_it).second);
    }
  }
  return neighbours;
}

void auditor::get_neighbour_data(std::string cluster) {
  std::string error;
  try {
    parse_bold_data(cluster);
  } catch (std::exception &e) {
    std::string exception_message(e.what());
    error = exception_message;
  }
  if (!error.empty()) {
    _errors.push_back(error);
  }
}

std::string auditor::check_connectivity(datatypes::species &species,
                                        datatypes::dataset &data,
                                        double max_distance) {
  size_t ctr, num_components;
  auto grade = "E";
  const std::unordered_set<std::string> clusters = species.getClusters();
  ctr = 0;
  for (auto &pair : data) {
    auto &current_clusters = pair.second.getClusters();
    if (utils::hasIntersection(clusters, current_clusters)) {
      ctr += 1;
      if (ctr > 1)
        return grade;
    }
  }

  auto cluster_begin = clusters.begin();
  auto cluster_end = clusters.end();
  auto neighbours = get_neighbours(clusters);
  if (neighbours.size() != clusters.size() && species.getGrade() != "Z") {
    _engine->inc_tasks(clusters.size());
    for (auto &cluster : clusters) {
      _engine->run_task([this, cluster]() { get_neighbour_data(cluster); });
    }
    return "Z";
  } else if (neighbours.size() != clusters.size() &&
             species.getGrade() == "Z") {
    return grade;
  } else {
    int i = 0;
    datatypes::undirected_graph graph(neighbours.size());
    for (auto &neighbour : neighbours) {
      auto item = clusters.find(neighbour.clusterB);
      if (item != cluster_end && neighbour.distance <= max_distance) {
        size_t ind = std::distance(cluster_begin, item);
        boost::add_edge(i, ind, neighbour.distance, graph);
      }
      i++;
    }
    std::vector<int> component(neighbours.size());
    num_components = boost::connected_components(graph, &component[0]);
    if (num_components == 1) {
      grade = "C";
    }
    return grade;
  }
}

void auditor::audit_species(std::string name, datatypes::dataset &data) {
  auto &species = data[name];
  int min_sources = _params.min_sources;
  int min_size = _params.min_size;
  double max_distance = _params.max_distance;
  std::string grade = "D";
  int size = species.sourcesCount();
  if (size >= min_sources) {
    grade = "E";
    if (species.clustersCount() == 1) {
      const std::string &bin = species.getFirstCluster();
      if (is_shared_bin(data, bin)) {
        int specimens_size = species.recordCount();
        grade = specimens_size >= min_size ? "A" : "B";
      }
    } else {
      grade = check_connectivity(species, data, max_distance);
    }
  }
  species.setGrade(grade);
}

void auditor::execute(datatypes::dataset &data) {
  _errors.clear();
  _engine->set_completed_tasks(0);
  _engine->set_tasks(data.size());
  //   _tasks = data.size();
  //   _c_tasks = 0;

  for (auto &pair : data) {
    std::string species = pair.first;
    _engine->run_task([&data, this, spec{std::move(species)}]() {
      audit_species(spec, data);
    });
  }

  _engine->wait();

  for (auto &pair : data) {
    if (pair.second.getGrade() == "Z") {
      std::string species = pair.first;
      _engine->inc_tasks();
      _engine->run_task([&data, this, spec{std::move(species)}]() {
        audit_species(spec, data);
      });
    }
  }
  _engine->wait();
}
void auditor::execute_with_distances(datatypes::dataset &data,
                                     datatypes::distance_matrix &distances) {
  _matrix = distances;
  execute(data);
}
} // namespace execution
} // namespace ispecid