#ifndef ENGINE_H
#define ENGINE_H
#include <condition_variable>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <atomic>
#include <cfloat>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <thread>
#include "cptl.h"
namespace ispecid
{
  namespace execution
  {
    class Engine
    {
    public:
      Engine(unsigned int cores)
      {
        unsigned int available_cores = std::thread::hardware_concurrency();
        if (cores > available_cores)
        {
          cores = available_cores;
        }
        m_pool = std::make_unique<ctpl::thread_pool>(cores);
      };

      ~Engine()
      {
        m_pool->stop(true);
      };

      template <typename Func>
      void runTask(Func&& f)
      {
        m_tasks++;
        m_results.push_back(m_pool->push(f));
      }

      void Wait()
      {
        auto ul = std::unique_lock<std::mutex>(m_mtx);
        m_cv.wait(ul, [&]()
                 { return m_tasks == m_completedTasks; });
        ul.unlock();
      }

      void setCompletedTasks(int tasks){m_completedTasks = tasks;}
      void setTasks(int tasks){m_tasks = tasks;}

    private:
      std::unique_ptr<ctpl::thread_pool> m_pool;
      std::condition_variable m_cv;
      std::mutex m_mtx;
      std::atomic<int> m_completedTasks;
      std::atomic<int> m_tasks;
      std::vector<std::future<void>> m_results;
    };
  } // namespace execution
} // namespace ispecid
#endif // ENGINE_H
