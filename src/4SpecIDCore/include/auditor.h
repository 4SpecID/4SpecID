#ifndef AUDITOR_H
#define AUDITOR_H
#include "engine_algorithm.h"
namespace ispecid {
namespace execution {

class auditor : public ialgorithm {
private:
  datatypes::grading_parameters _params;
  datatypes::distance_matrix _matrix;

  bool is_shared_bin(datatypes::dataset &data, const std::string &bin);
  void parse_bold_data(std::string cluster);
  std::vector<datatypes::neighbour>
  get_neighbours(const std::unordered_set<std::string> &clusters);
  std::string check_connectivity(datatypes::species &species,
                                datatypes::dataset &data, double max_distance);
  void audit_species(std::string species, datatypes::dataset &data);
  void get_neighbour_data(std::string cluster);

public:
  auditor(iengine *engine) : ialgorithm(engine) {}
  auditor(datatypes::grading_parameters parameters, iengine *engine)
      : ialgorithm(engine), _params(parameters)  {}
  auditor(datatypes::grading_parameters parameters,
          datatypes::distance_matrix &matrix, iengine *engine)
      : ialgorithm(engine), _params(parameters), _matrix(matrix) {}
  ~auditor(){};
  datatypes::distance_matrix get_distances(){ return _matrix;}
  void execute(datatypes::dataset &data);
  void execute_with_distances(datatypes::dataset &data,
                              datatypes::distance_matrix &distances);
};

} // namespace execution
} // namespace ispecid
#endif
