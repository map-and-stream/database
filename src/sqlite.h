#pragma once

#include "database.h"
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>

class SQLite : public IDatabase {
  public:
    explicit SQLite(ConnectionConfig cfg);
    ~SQLite() override;

    bool open() override;
    void close() override;
    bool is_open() const override;

    bool insert(const std::string& query, const std::vector<std::string>& values) override;
    bool update(const std::string& query, const std::vector<std::string>& params) override;
    bool remove(const std::string& query, const std::vector<std::string>& params) override;
    QueryResult select(const std::string& query, const std::vector<std::string>& params = {}) override;

    SQLite(const SQLite&) = delete;
    SQLite& operator=(const SQLite&) = delete;

            // Movable
    SQLite(SQLite&&) = default;
    SQLite& operator=(SQLite&&) = default;

  private:
    sqlite3* db_ = nullptr;
    bool executeQuery(const std::string& query, const std::vector<std::string>& params, bool returnsData, QueryResult* result = nullptr);
};
