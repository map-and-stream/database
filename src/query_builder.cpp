#include "query_builder.h"
#include <algorithm>

// ===== SELECT =====
QueryBuilder QueryBuilder::select(const std::vector<std::string>& columns) {
    QueryBuilder qb(Type::SELECT);
    qb.selectColumns_ = columns;
    return qb;
}

QueryBuilder& QueryBuilder::from(const std::string& table) {
    table_ = table;
    return *this;
}

QueryBuilder& QueryBuilder::where(const std::string& condition) {
    whereClause_ = condition;
    return *this;
}

QueryBuilder& QueryBuilder::orderBy(const std::string& column, bool ascending) {
    orderByClause_ = column + (ascending ? " ASC" : " DESC");
    return *this;
}

QueryBuilder& QueryBuilder::limit(int count) {
    limit_ = count;
    return *this;
}

QueryBuilder& QueryBuilder::offset(int count) {
    offset_ = count;
    return *this;
}

// ===== INSERT =====
QueryBuilder QueryBuilder::insertInto(const std::string& table) {
    QueryBuilder qb(Type::INSERT);
    qb.table_ = table;
    return qb;
}

QueryBuilder& QueryBuilder::columns(const std::vector<std::string>& cols) {
    insertColumns_ = cols;
    return *this;
}

QueryBuilder& QueryBuilder::values(const std::vector<std::string>& vals) {
    insertValues_ = vals;
    return *this;
}

// ===== UPDATE =====
QueryBuilder QueryBuilder::update(const std::string& table) {
    QueryBuilder qb(Type::UPDATE);
    qb.table_ = table;
    return qb;
}

QueryBuilder& QueryBuilder::set(const std::string& column, const std::string& value) {
    updateSets_.emplace_back(column, value);
    return *this;
}

// ===== DELETE =====
QueryBuilder QueryBuilder::del(const std::string& table) {
    QueryBuilder qb(Type::DELETE);
    qb.table_ = table;
    return qb;
}

// ===== BUILD =====
std::pair<std::string, std::vector<std::string>> QueryBuilder::build() const {
    std::ostringstream sql;
    std::vector<std::string> params;

    switch (type_) {
        case Type::SELECT: {
            sql << "SELECT ";
            for (size_t i = 0; i < selectColumns_.size(); ++i) {
                if (i > 0) sql << ", ";
                sql << selectColumns_[i];
            }
            sql << " FROM " << table_;

            if (!whereClause_.empty()) {
                sql << " WHERE " << whereClause_;
                // Note: whereClause_ should contain placeholders like "name = ?"
            }

            if (!orderByClause_.empty()) {
                sql << " ORDER BY " << orderByClause_;
            }

            if (limit_.has_value()) {
                sql << " LIMIT ?";
                params.push_back(std::to_string(limit_.value()));
            }

            if (offset_.has_value()) {
                sql << " OFFSET ?";
                params.push_back(std::to_string(offset_.value()));
            }
            break;
        }

        case Type::INSERT: {
            sql << "INSERT INTO " << table_ << " (";
            for (size_t i = 0; i < insertColumns_.size(); ++i) {
                if (i > 0) sql << ", ";
                sql << insertColumns_[i];
            }
            sql << ") VALUES (";
            for (size_t i = 0; i < insertValues_.size(); ++i) {
                if (i > 0) sql << ", ";
                sql << "?";
                params.push_back(insertValues_[i]);
            }
            sql << ")";
            break;
        }

        case Type::UPDATE: {
            sql << "UPDATE " << table_ << " SET ";
            for (size_t i = 0; i < updateSets_.size(); ++i) {
                if (i > 0) sql << ", ";
                sql << updateSets_[i].first << " = ?";
                params.push_back(updateSets_[i].second);
            }

            if (!whereClause_.empty()) {
                sql << " WHERE " << whereClause_;
                // whereClause_ must contain placeholders (e.g., "id = ?")
                // You'd need to append those params separately (see note below)
            }
            break;
        }

        case Type::DELETE: {
            sql << "DELETE FROM " << table_;
            if (!whereClause_.empty()) {
                sql << " WHERE " << whereClause_;
                // whereClause_ must contain placeholders
            }
            break;
        }
    }

    return {sql.str(), params};
}
