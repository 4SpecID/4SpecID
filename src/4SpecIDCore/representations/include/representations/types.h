#pragma once
#include <map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include "species.h"
#include "distance.h"

namespace ispecid
{
  namespace representations
  {

    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
                                  boost::no_property,
                                  boost::property<boost::edge_weight_t, double>>
        UDGraph;
    typedef std::unordered_map<std::string, Species> Dataset;
    typedef std::map<std::string, Distance> DistanceMatrix;
  } // namespace representations
} // namespace ispecid