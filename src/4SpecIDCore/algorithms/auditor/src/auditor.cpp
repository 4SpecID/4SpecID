#include "auditor/auditor.h"
#include <network/network.h>
#include <limits>
namespace ispecid
{
namespace execution
{

bool Auditor::isSharedBin(representations::Dataset& data, const std::string& bin)
{
  std::unordered_set<std::string_view> unique_set;
  size_t count = 0;
  for (auto& [key, species] : data)
  {
    if (species.hasCluster(bin) && (++count) == 2)
    {
      return false;
    }
  }
  return true;
}


void Auditor::parseDistance(std::string cluster)
{
  representations::Distance distance;
  std::string url("http://v4.boldsystems.org/index.php/Public_BarcodeCluster?clusteruri=" + std::string(cluster));
  std::string page = network::get_page(url.c_str());
  
  boost::smatch char_matches;
  bool no_distance, no_name;
  no_distance = no_name = false;
  distance.m_clusterA = cluster;
  if (boost::regex_search(page, char_matches, distanceRegex))
  {
    distance.m_value = std::stod(char_matches[1]) ;
  }
  else
  {
    distance.m_value = DBL_MAX;
    no_distance = true;
  }
  if (boost::regex_search(page, char_matches, clusterRegex))
  {
    distance.m_clusterB = char_matches[1];
  }
  else
  {
    distance.m_clusterB = "UNDEFINED";
    no_name = true;
  }
  auto pair = std::make_pair(cluster, distance);
  m_matrix.insert(pair);

  if (no_name)
    throw std::runtime_error("distance not found when retrieving " + cluster);
  else if (no_distance)
    throw std::runtime_error("Distance not found when retrieving " + cluster);
}

std::vector<representations::Distance> Auditor::getClusterDistances(const std::unordered_set<std::string>& clusters)
{
  std::vector<representations::Distance> distances;
  auto endIt = m_matrix.end();
  for (auto& cluster : clusters)
  {
    auto it = m_matrix.find(cluster);
    if (it != endIt)
    {
      distances.push_back((*it).second);
    }
  }
  return distances;
}

void Auditor::fetchDistances(const std::string& cluster)
{
  std::string error;
  try
  {
    parseDistance(cluster);
  }
  catch (std::exception& e)
  {
    std::string exception_message(e.what());
    m_errors.push_back(exception_message);
  }
}

std::string Auditor::checkConnectivity(representations::Species& species, representations::Dataset& data, double max_distance)
{
  size_t counter = 0;
  auto grade = "E";
  auto clusters = species.getClusters();
  for (auto& [key, value]: data)
  {
    auto& current = value.getClusters();
    if (utils::hasIntersection(clusters, current))
    {
      if (++counter > 1)
        return grade;
    }
  }

  
  auto distances = getClusterDistances(clusters);
  if (distances.size() != clusters.size() && species.getGrade() != "Z")
  {
    for (auto& cluster : clusters)
    {
      m_engine->runTask([this, &cluster]([[maybe_unused]] int id) { fetchDistances(cluster); });
    }
    return "Z";
  }
  else if (distances.size() != clusters.size() && species.getGrade() == "Z")
  {
    return grade;
  }
  else
  {
    AB
    AD
    CE
    int i = 0;
    representations::UDGraph graph(distances.size());
    std::vector<int> component(distances.size());
    auto begin = clusters.begin();
    auto end = clusters.end();
    for (auto& distance : distances)
    {
      auto item = clusters.find(distance.m_clusterB);
      if (item != end && distance.m_value <= max_distance)
      {
        size_t diff = std::distance(begin, item);
        boost::add_edge(i++, diff, distance.m_value, graph);
      }
    }
    if (boost::connected_components(graph, &component[0]) == 1)
    {
      grade = "C";
    }
    return grade;
  }
}

void Auditor::auditSpecies(std::string name, representations::Dataset& data)
{
  auto& species = data[name];
  int min_sources = m_params.minSources;
  int min_size = m_params.minRecords;
  double max_distance = m_params.maxDistance;
  std::string grade = "D";
  int size = species.getSourcesSize();
  if (size >= min_sources)
  {
    grade = "E";
    if (species.getClustersSize() == 1)
    {
      const std::string& bin = species.getFirstCluster();
      if (isSharedBin(data, bin))
      {
        int specimens_size = species.size();
        grade = specimens_size >= min_size ? "A" : "B";
      }
    }
    else
    {
      grade = checkConnectivity(species,data,max_distance);
    }
  }
  species.setGrade(grade);
}

void Auditor::execute(representations::Dataset& data)
{
  m_errors.clear();
  m_engine->setCompletedTasks(0);
  m_engine->setTasks(data.size());

  for (auto& [key, value] : data)
  {
    m_engine->runTask([this, species{std::move(key)}, &data]([[maybe_unused]] int id) { auditSpecies(species, data); });
  }

  m_engine->Wait();

  for (auto& [key, value] : data)
  {
    if (value.getGrade() == "Z")
    {
      m_engine->runTask([this, species{std::move(key)}, &data]([[maybe_unused]] int id) { auditSpecies(species, data); });
    }
  }
  m_engine->Wait();
}
} // namespace iexecution
} // namespace ispecid
