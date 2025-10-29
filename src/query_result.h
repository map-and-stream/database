#pragma once

#include <string>
#include <vector>

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
