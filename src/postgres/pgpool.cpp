#include "pgpool.h"

#include <iostream>


PGPool::PGPool(const std::string& connInfo, std::size_t poolSize) : m_connInfo{connInfo}, m_poolSize{poolSize} {

    if (poolSize == 0)
        throw std::invalid_argument("poolSize must be > 0");

    for (std::size_t i = 0; i < m_poolSize; i++) {
        auto conn = std::make_shared<pqxx::connection>(m_connInfo);
        if (!conn->is_open()) {
            throw std::runtime_error("Failed to create connection");
            m_isOpen = false;
        }
        else 
        {
            m_isOpen = true;
        }
        m_pool.push(conn);
    }
}

std::shared_ptr<pqxx::connection> PGPool::connection() {
    std::unique_lock<std::mutex> lock(m_mutex);

    // if pool is empty, then wait until it notifies back
    while (m_pool.empty()) {
        m_condition.wait(lock);
    }

    // get new connection in queue
    auto conn = m_pool.front();
    // immediately pop as we will use it now
    m_pool.pop();

    return conn;
}

void PGPool::freeConnection(std::shared_ptr<pqxx::connection> conn) {
    std::unique_lock<std::mutex> lock(m_mutex);

    // push a new connection into a pool
    m_pool.push(conn);

    // unlock mutex
    lock.unlock();

    // notify one of thread that is waiting
    m_condition.notify_one();
}

bool PGPool::is_open()
{
    return m_isOpen.load();
}

PGPool::~PGPool() {
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_pool.empty()) {
        auto conn = m_pool.front();
        m_pool.pop();
        conn->close();
    }
}