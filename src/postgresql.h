#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>  // This includes the full definition of pqxx::connection
#include <string>
#include <vector>
#include "database.h"

class PostgreSQL :public IDatabase{
  public:
    PostgreSQL(ConnectionConfig cfg) : IDatabase(cfg) {}

    bool open() override;
    void close() override;
    bool is_open() const override;
    pqxx::connection* get();

    bool insert(const std::string& query, const std::vector<std::string>& values) override;
    QueryResult select(const std::string& query, const std::vector<std::string>& params = {}) override;
    void printResult(const pqxx::result& res) {
        for (const auto& row : res) {
            for (const auto& field : row) {
                std::cout << field.c_str() << "\t";  // print column value
            }
            std::cout << "\n";
        }
    }
    bool update(const std::string& query, const std::vector<std::string>& params) override;
    bool remove(const std::string& query, const std::vector<std::string>& params) override;

    // Non-copyable
    PostgreSQL(const PostgreSQL&) = delete;
    PostgreSQL& operator=(const PostgreSQL&) = delete;

    // Movable
    PostgreSQL(PostgreSQL&&) = default;
    PostgreSQL& operator=(PostgreSQL&&) = default;

    ~PostgreSQL();

  private:
    std::unique_ptr<pqxx::connection> connection_;
    std::string conninfo_;
};
