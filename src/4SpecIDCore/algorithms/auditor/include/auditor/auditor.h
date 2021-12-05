#pragma once
#include <algorithm.h>
#include <representations/gradingParameters.h>
#include <representations/types.h>
#include "utils.h"
namespace ispecid
{
namespace execution
{

class Auditor : public Algorithm
{
private:
  representations::GradingParameters m_params;
  representations::DistanceMatrix m_matrix;

  const boost::regex distanceRegex{"Distance to Nearest Neighbor:</th>\\s*<td>(\\d+.\\d+)%.*</td>"};
  const boost::regex clusterRegex{"Nearest BIN URI:</th>\\s*<td>(.*?)</td>"};


  bool isSharedBin(representations::Dataset& data, const std::string& bin);
  std::string checkConnectivity(representations::Species& species, representations::Dataset& data, double max_distance);
  void parseDistance(std::string cluster);
  std::vector<representations::Distance> getClusterDistances(const std::unordered_set<std::string>& clusters);
  void fetchDistances(const std::string& cluster);
  void auditSpecies(std::string species, representations::Dataset& data);

public:
  Auditor(Engine* engine)
      : Algorithm(engine)
  {
  }
  Auditor(representations::GradingParameters parameters, Engine* engine)
      : Algorithm(engine)
      , m_params(parameters)
  {
  }
  Auditor(representations::GradingParameters parameters, representations::DistanceMatrix& matrix, Engine* engine)
      : Algorithm(engine)
      , m_params(parameters)
      , m_matrix(matrix)
  {
  }
  ~Auditor(){};
  representations::DistanceMatrix getDistances() { return m_matrix; }
  void setDistances(representations::DistanceMatrix& distances) { m_matrix = distances; }
  void execute(representations::Dataset& data);
};

} // namespace iexecution
} // namespace ispecid