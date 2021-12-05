#pragma once
#include <string>

namespace ispecid
{
  namespace representations
  {

    struct Distance
    {
      std::string m_clusterA;
      std::string m_clusterB;
      double m_value;

      Distance(){};
      Distance(std::string &clusterA, std::string &clusterB, double value)
          : m_clusterA(clusterA), m_clusterB(clusterB), m_value(value){};
    };
  } // namespace representations
} // namespace ispecid