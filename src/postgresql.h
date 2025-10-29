#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "database.h"

class PostgreSQL : public IDatabase {
  public:
    PostgreSQL(ConnectionConfig cfg) : IDatabase(std::move(cfg)) {}

    bool open() override;
    void close() override;
    bool is_open() const override;

    bool insert(const std::string& query, const std::vector<std::string>& values) override;
    bool update(const std::string& query, const std::vector<std::string>& params) override;
    bool remove(const std::string& query, const std::vector<std::string>& params) override;
    QueryResult select(const std::string& query,
                       const std::vector<std::string>& params = {}) override;

    // Non-copyable
    PostgreSQL(const PostgreSQL&) = delete;
    PostgreSQL& operator=(const PostgreSQL&) = delete;

    // Movable
    PostgreSQL(PostgreSQL&&) = default;
    PostgreSQL& operator=(PostgreSQL&&) = default;

    ~PostgreSQL();

  private:
    std::unique_ptr<pqxx::connection> connection_;
};
