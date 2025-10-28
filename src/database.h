#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "config.h"
#include <optional>

class QueryResult {
  public:
    using Row = std::vector<std::string>;
    using Table = std::vector<Row>;

    QueryResult() = default;
    QueryResult(Table table, std::vector<std::string> columns)
        : table_(std::move(table)), columns_(std::move(columns)) {}

    bool empty() const { return table_.empty(); }
    size_t rows() const { return table_.size(); }
    size_t cols() const { return columns_.size(); }
    const std::vector<std::string>& columns() const { return columns_; }
    const Table& data() const { return table_; }

            // Optional: helper to get cell by (row, col)
    std::optional<std::string> at(size_t row, size_t col) const {
        if (row < table_.size() && col < table_[row].size()) {
            return table_[row][col];
        }
        return std::nullopt;
    }

  private:
    Table table_;
    std::vector<std::string> columns_;
};

class IDatabase {
  public:
    virtual ~IDatabase() = default;
    IDatabase(ConnectionConfig cfg) : config(cfg) {}

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool is_open() const = 0;

    virtual bool insert(const std::string& query, const std::vector<std::string>& values) = 0;
    virtual QueryResult select(const std::string& query, const std::vector<std::string>& params = {}) = 0;
    virtual bool update(const std::string& query, const std::vector<std::string>& params) = 0;
    virtual bool remove(const std::string& query, const std::vector<std::string>& params) = 0;
  protected:
    ConnectionConfig config;
};
