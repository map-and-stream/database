// Implementation

#include "postgresql.h"

#include <iostream>
#include <pqxx/pqxx>
#include <stdexcept>

bool PostgreSQL::open() {
    if (connection_) {
        return true;  // Already open
    }
    try {
        connection_ = std::make_unique<pqxx::connection>(conninfo_);
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

pqxx::connection* PostgreSQL::get() {
    return connection_.get();
}

PostgreSQL::~PostgreSQL() {
    close();
}

bool PostgreSQL::insert(const std::string& query, const std::vector<std::string>& values) {
try {
    pqxx::work txn(*connection_.get());

    // Execute the query with parameters
    pqxx::result res = txn.exec_params(query, pqxx::prepare::make_dynamic_params(values.begin(), values.end()));

    txn.commit();
    return true;
} catch (const std::exception& e) {
    std::cerr << "Insert failed: " << e.what() << std::endl;
    return false;
}
}

pqxx::result PostgreSQL::select(
    const std::string& query, 
    const std::vector<std::string>& params
) {
    try {
        pqxx::work txn(*connection_.get());

        // Execute the query with parameters
        pqxx::result res;
        if (params.empty()) {
            res = txn.exec(query);
        } else {
            res = txn.exec_params(
                query,
                pqxx::prepare::make_dynamic_params(params.begin(), params.end())
            );
        }

        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::cerr << "SELECT failed: " << e.what() << std::endl;
        return pqxx::result{}; // empty result on failure
    }
}