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
    if (!is_open()) {
        std::cerr << "❌ Cannot insert: database not open.\n";
        return false;
    }

    try {
        pqxx::work txn(*connection_.get());

        // Execute the query with parameters
        pqxx::result res = txn.exec_params(
            query, pqxx::prepare::make_dynamic_params(values.begin(), values.end()));

        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Insert failed: " << e.what() << std::endl;
        return false;
    }
}

pqxx::result PostgreSQL::select(const std::string& query, const std::vector<std::string>& params) {
    try {
        pqxx::work txn(*connection_.get());

        // Execute the query with parameters
        pqxx::result res;
        if (params.empty()) {
            res = txn.exec(query);
        } else {
            res = txn.exec_params(query,
                                  pqxx::prepare::make_dynamic_params(params.begin(), params.end()));
        }

        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::cerr << "SELECT failed: " << e.what() << std::endl;
        return pqxx::result{};  // empty result on failure
    }
}

bool PostgreSQL::update(const std::string& query, const std::vector<std::string>& params) {
    if (!is_open()) {
        std::cerr << "❌ Cannot update: database not open.\n";
        return false;
    }

    try {
        pqxx::work txn(*connection_.get());
        if (params.empty())
            txn.exec(query);
        else
            txn.exec_params(query, pqxx::prepare::make_dynamic_params(params));
        txn.commit();
        std::cout << "✅ Update successful.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Update failed: " << e.what() << "\n";
        return false;
    }
}

bool PostgreSQL::remove(const std::string& query, const std::vector<std::string>& params) {
    if (!is_open()) {
        std::cerr << "❌ Cannot delete: database not open.\n";
        return false;
    }

    try {
        pqxx::work txn(*connection_.get());
        if (params.empty())
            txn.exec(query);
        else
            txn.exec_params(query, pqxx::prepare::make_dynamic_params(params));
        txn.commit();
        std::cout << "🗑️  Delete successful.\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Delete failed: " << e.what() << "\n";
        return false;
    }
}
