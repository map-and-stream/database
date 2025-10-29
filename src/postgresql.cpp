#include "postgresql.h"

#include <iostream>
#include <pqxx/pqxx>
#include <stdexcept>

bool PostgreSQL::open() {
    if (connection_) {
        return true;  // Already open
    }
    try {
        connection_ = std::make_unique<pqxx::connection>(config.toPostgresConnection());
        return connection_->is_open();
    } catch (const std::exception& e) {
        std::cerr << "⚠ Other error: " << e.what() << "\n";
        connection_.reset();
        return false;
    }
}

void PostgreSQL::close() {
    if (connection_) {
        // connection_.release();
        connection_.reset();
    }
}

bool PostgreSQL::is_open() const {
    return connection_ && connection_->is_open();
}

PostgreSQL::~PostgreSQL() {
    close();
}

bool PostgreSQL::insert(const QueryBuilder& qb) {
    if (!is_open()) {
        std::cerr << "❌ Cannot insert: database not open.\n";
        return false;
    }

    try {
        pqxx::work txn(*connection_.get());

        // Execute the query with parameters
        pqxx::result res = txn.exec_params(qb.str());

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Insert failed: " << e.what() << std::endl;
        return false;
    }
}

QueryResult convert_result(const pqxx::result& res) {
    if (res.empty()) {
        // Return empty result with column names (if available)
        std::vector<std::string> columns;
        if (res.columns() > 0) {
            columns.reserve(res.columns());
            for (pqxx::row_size_type i = 0; i < res.columns(); ++i) {
                columns.push_back(std::string(res.column_name(i)));
            }
        }
        return QueryResult({}, std::move(columns));
    }

    // Extract column names
    std::vector<std::string> columns;
    columns.reserve(res.columns());
    for (pqxx::row_size_type i = 0; i < res.columns(); ++i) {
        columns.push_back(std::string(res.column_name(i)));
    }

    // Extract rows
    QueryResult::Table table;
    table.reserve(res.size());

    for (const auto& row : res) {
        QueryResult::Row r;
        r.reserve(row.size());
        for (const auto& field : row) {
            // Convert field to string; handle NULLs safely
            if (field.is_null()) {
                r.emplace_back("NULL");  // or use empty string "" based on your policy
            } else {
                r.emplace_back(std::string(field.c_str()));
            }
        }
        table.push_back(std::move(r));
    }

    return QueryResult(std::move(table), std::move(columns));
}

QueryResult PostgreSQL::select(const QueryBuilder& qb) {
    try {
        pqxx::work txn(*connection_.get());

        // Execute the query with parameters
        pqxx::result res;
        res = txn.exec(qb.str());

        txn.commit();
        return convert_result(res);
    } catch (const std::exception& e) {
        std::cerr << "SELECT failed: " << e.what() << std::endl;
        return convert_result(pqxx::result{});  // empty result on failure
    }
}

bool PostgreSQL::update(const QueryBuilder& qb) {
    if (!is_open()) {
        std::cerr << "❌ Cannot update: database not open.\n";
        return false;
    }

    try {
        pqxx::work txn(*connection_.get());
        txn.exec(qb.str());
        txn.commit();
        std::cout << "✅ Update successful.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Update failed: " << e.what() << "\n";
        return false;
    }
}

bool PostgreSQL::remove(const QueryBuilder& qb) {
    if (!is_open()) {
        std::cerr << "❌ Cannot delete: database not open.\n";
        return false;
    }

    try {
        pqxx::work txn(*connection_.get());

        txn.exec(qb.str());
        
        txn.commit();
        std::cout << "🗑️  Delete successful.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Delete failed: " << e.what() << "\n";
        return false;
    }
}
