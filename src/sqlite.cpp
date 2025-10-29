#include "sqlite.h"

#include <iostream>
#include <stdexcept>

SQLite::SQLite(ConnectionConfig cfg) : IDatabase(std::move(cfg)) {}

SQLite::~SQLite() {
    close();
}

bool SQLite::open() {
    if (is_open())
        return true;

    // config.path should contain the SQLite DB file path
    int rc = sqlite3_open(config_.path.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open SQLite database: " << sqlite3_errmsg(db_) << std::endl;
        close();
        return false;
    }
    return true;
}

void SQLite::close() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool SQLite::is_open() const {
    return db_ != nullptr;
}

bool SQLite::insert(const QueryBuilder& qb) {
    return executeQuery(qb, false);
}

QueryResult SQLite::select(const QueryBuilder& qb) {
    QueryResult result;
    executeQuery(qb, true, &result);
    return result;
}

bool SQLite::update(const QueryBuilder& qb) {
    return executeQuery(qb, false);
}

bool SQLite::remove(const QueryBuilder& qb) {
    return executeQuery(qb, false);
}

bool SQLite::executeQuery(const QueryBuilder& qb, bool returnsData, QueryResult* result) {
    if (!is_open() && !open()) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db_, qb.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (prepare): " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    if (returnsData) {
        // Fetch column names
        int colCount = sqlite3_column_count(stmt);
        std::vector<std::string> columns;
        for (int i = 0; i < colCount; ++i) {
            const char* name = sqlite3_column_name(stmt, i);
            columns.emplace_back(name ? name : "");
        }

        // Fetch rows
        QueryResult::Table table;
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            QueryResult::Row row;
            for (int i = 0; i < colCount; ++i) {
                const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                row.emplace_back(text ? text : "");
            }
            table.push_back(std::move(row));
        }

        if (rc != SQLITE_DONE) {
            std::cerr << "SQL error (step): " << sqlite3_errmsg(db_) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }

        *result = QueryResult(std::move(table), std::move(columns));
    } else {
        // Non-SELECT query
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "SQL error (step): " << sqlite3_errmsg(db_) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
    }

    sqlite3_finalize(stmt);
    return true;
}
