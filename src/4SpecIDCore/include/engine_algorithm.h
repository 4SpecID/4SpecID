#ifndef ENGINE_ALGO_H
#define ENGINE_ALGO_H
#include "datatypes.h"
#include "iengine.h"
namespace ispecid { namespace execution{
class ialgorithm {
protected:
  iengine *_engine;
  std::vector<std::string> _errors;

public:
  ialgorithm(iengine *engine): _engine(engine) {};
  virtual ~ialgorithm() = default;
  
  std::vector<std::string> errors() { return _errors; }
  virtual void execute(datatypes::dataset &data) = 0;
};
}
} // namespace ispecid

#endif