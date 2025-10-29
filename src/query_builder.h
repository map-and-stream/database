#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <optional>

class QueryBuilder {
  public:
    // ===== SELECT =====
    static QueryBuilder select(const std::vector<std::string>& columns = {"*"});

    QueryBuilder& from(const std::string& table);
    QueryBuilder& where(const std::string& condition);
    QueryBuilder& orderBy(const std::string& column, bool ascending = true);
    QueryBuilder& limit(int count);
    QueryBuilder& offset(int count);

            // ===== INSERT =====
    static QueryBuilder insertInto(const std::string& table);
    QueryBuilder& columns(const std::vector<std::string>& cols);
    QueryBuilder& values(const std::vector<std::string>& vals);

            // ===== UPDATE =====
    static QueryBuilder update(const std::string& table);
    QueryBuilder& set(const std::string& column, const std::string& value);
    // Note: For multiple sets, call .set() multiple times

            // ===== DELETE =====
    static QueryBuilder del(const std::string& table); // 'delete' is keyword

            // Build final query and parameters
    std::pair<std::string, std::vector<std::string>> build() const;

  private:
    enum class Type { SELECT, INSERT, UPDATE, DELETE };

    Type type_;
    std::string table_;
    std::vector<std::string> selectColumns_;
    std::vector<std::string> insertColumns_;
    std::vector<std::string> insertValues_;
    std::vector<std::pair<std::string, std::string>> updateSets_; // col -> value
    std::string whereClause_;
    std::string orderByClause_;
    std::optional<int> limit_;
    std::optional<int> offset_;

    explicit QueryBuilder(Type type) : type_(type) {}
};
