#pragma once
#include <string>


namespace ispecid
{
  namespace representations
  {
    struct GradingParameters
    {
      int minSources = 2;
      int minRecords = 3;
      double maxDistance = 2.0;

      GradingParameters(){};
      GradingParameters(int minSources, int minRecords, double maxDistance) : 
        minSources(minSources), minRecords(minRecords), maxDistance(maxDistance)
      { }
    };
  } // namespace representations
} // namespace ispecid