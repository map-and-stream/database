#ifndef PGPOOL_H
#define PGPOOL_H

#include <mutex>
#include <memory>
#include <condition_variable>
#include <queue>
#include <cstdint>
#include <pqxx/pqxx>
#include <atomic>

class PGPool
{
public:

  PGPool(const std::string& connInfo, std::size_t poolSize);

  PGPool(const PGPool&) = delete;
  PGPool& operator=(const PGPool&) = delete;

  std::shared_ptr<pqxx::connection> connection();
  void freeConnection(std::shared_ptr<pqxx::connection>);
  bool is_open();


  ~PGPool();


private:
    
  std::mutex m_mutex;
  std::condition_variable m_condition;
  std::queue<std::shared_ptr<pqxx::connection>> m_pool;

  std::size_t m_poolSize = 0;
  std::string m_connInfo;
  std::atomic<bool> m_isOpen{false};
  
};

#endif
