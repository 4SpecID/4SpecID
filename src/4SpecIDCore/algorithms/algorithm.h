#ifndef ENGINE_ALGO_H
#define ENGINE_ALGO_H
#include <representations/types.h>
#include <engine.h>
namespace ispecid { namespace execution{
class Algorithm {
protected:
  Engine *m_engine;
  std::vector<std::string> m_errors;

public:
  Algorithm(Engine *engine): m_engine(engine) {};
  virtual ~Algorithm() = default;
  
  std::vector<std::string> errors() { return m_errors; }
  virtual void execute(representations::Dataset &data) = 0;
};
}
} // namespace ispecid

#endif