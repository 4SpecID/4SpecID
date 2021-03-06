#ifndef DATATYPES_H
#define DATATYPES_H
#include "record.h"
#include "species.h"
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <float.h>
#ifndef Q_MOC_RUN
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#endif

namespace ispecid {
namespace datatypes {

struct GradingParameters {
  int minSources = 2;
  int minRecords = 3;
  double maxDistance = 2.0;

  GradingParameters(){};
  GradingParameters(int minSources, int minRecords, double maxDistance):
      minSources(minSources),
      minRecords(minRecords),
      maxDistance(maxDistance)
  {}
};

struct Project{
    GradingParameters parameters;
    std::string name;

    Project() : parameters(), name(""){};
    Project(int minSources, int minRecords, double maxDistance, std::string name):
        parameters(minSources, minRecords, maxDistance),
        name(name)
    {}
};

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, double>>
    undirected_graph;

class neighbour {
  private:
  std::string clusterA;
  std::string clusterB;
  double distance;

  public:
  neighbour() : clusterA(""), clusterB(""), distance(0.0) {};
  neighbour(std::string ac, std::string bc, double d)
    : clusterA(ac), clusterB(bc), distance(d)
  {};
  neighbour(const neighbour& n){
    clusterA = n.clusterA; 
    clusterB = n.clusterB; 
    distance = n.distance; 
  }

  neighbour& operator=(const neighbour& n){
    clusterA = n.clusterA; 
    clusterB = n.clusterB; 
    distance = n.distance; 
    return *this;
  }

  neighbour(neighbour&& n) noexcept {
    clusterA = std::move(n.clusterA); 
    clusterB = std::move(n.clusterB); 
    distance = std::exchange(n.distance, DBL_MAX);
  }

  neighbour& operator=(neighbour&& n) noexcept {
    clusterA = std::move(n.clusterA); 
    clusterB = std::move(n.clusterB); 
    distance = std::exchange(n.distance, DBL_MAX);
    return *this;
  }

  std::string getA() const { return clusterA;}
  std::string getB() const { return clusterB;}
  double getDistance() const { return distance;}

  void setA(std::string ac) { clusterA = ac;}
  void setB(std::string bc) { clusterB = bc;}
  void setDistance(double d) { distance = d;}

  ~neighbour(){}
};

typedef std::unordered_map<std::string, species> dataset;
typedef std::map<std::string, neighbour> distance_matrix;
} // namespace datatypes
} // namespace ispecid
#endif
