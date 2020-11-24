#ifndef IENGINE_H
#define IENGINE_H
#include <condition_variable>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#ifndef Q_MOC_RUN
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#endif
#include "datatypes.h"
#include "network.h"
#include "utils.h"
#include <atomic>
#include <cfloat>
#include <cstdio>
#include <limits>
#include <stdexcept>
namespace ispecid {
namespace execution {
class iengine {
public:
  iengine(int cores) {
    network::prepare_network();
    int available_cores = boost::thread::hardware_concurrency();
    if (cores > available_cores) {
      cores = available_cores;
    }
    pool = new boost::asio::thread_pool(cores);
  };
  ~iengine() {
    pool->join();
    delete pool;
  };

  void dec_tasks(int tasks) { _tasks -= tasks; }
  void inc_tasks(int tasks = 1) { _tasks += tasks; }
  void set_tasks(int tasks) { _tasks = tasks; }
  void set_completed_tasks(int c_tasks) { _c_tasks = c_tasks; }

  template <typename Func> void run_task(Func f) {
    boost::asio::post(*pool, [&, f]() {
      f();
      _c_tasks++;
      if (_c_tasks == _tasks) {
        _cv.notify_one();
      }
    });
  }
  void wait() {
    auto ul = std::unique_lock<std::mutex>(_mtx);
    _cv.wait(ul, [&]() { return _tasks == _c_tasks; });
    ul.unlock();
  }

private:
  boost::asio::thread_pool *pool;
  std::condition_variable _cv;
  std::mutex _mtx;
  std::atomic<int> _c_tasks;
  std::atomic<int> _tasks;
};
} // namespace execution
} // namespace ispecid
#endif // IENGINE_H
