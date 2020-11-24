#ifndef DATATYPES_H
#define DATATYPES_H
#include "record.h"
#include "species.h"
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#ifndef Q_MOC_RUN
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#endif

namespace ispecid {
namespace datatypes {

typedef struct grading_params_struct {
  int min_sources = 2;
  int min_size = 3;
  double max_distance = 2.0;
} grading_parameters;

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, double>>
    undirected_graph;

typedef struct neighbour_struct {
  std::string clusterA;
  std::string clusterB;
  double distance;
} neighbour;

typedef std::unordered_map<std::string, species> dataset;
typedef std::map<std::string, neighbour> distance_matrix;
} // namespace datatypes
} // namespace ispecid
#endif
