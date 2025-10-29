#pragma once

#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "config.h"
#include "query_result.h"
#include "querybuilder/query_builder.h"

enum class DatabaseType { PostgreSQL, sqlite };

class IDatabase {
  public:
    virtual ~IDatabase() = default;
    IDatabase(ConnectionConfig cfg) : config_(cfg) {}

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool is_open() const = 0;

    virtual bool insert(const QueryBuilder& qb) = 0;
    virtual bool update(const QueryBuilder& qb) = 0;
    virtual bool remove(const QueryBuilder& qb) = 0;
    virtual QueryResult select(const QueryBuilder& qb) = 0;

  protected:
    ConnectionConfig config_;
};
