#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "config.h"
#include <optional>
#include "query_result.h"


enum class DatabaseType {
    PostgreSQL,
    sqlite
};

class IDatabase {
  public:
    virtual ~IDatabase() = default;
    IDatabase(ConnectionConfig cfg) : config(cfg) {}

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool is_open() const = 0;

    virtual bool insert(const std::string& query, const std::vector<std::string>& values) = 0;
    virtual bool update(const std::string& query, const std::vector<std::string>& params) = 0;
    virtual bool remove(const std::string& query, const std::vector<std::string>& params) = 0;
    virtual QueryResult select(const std::string& query, const std::vector<std::string>& params = {}) = 0;
  protected:
    ConnectionConfig config;
};
