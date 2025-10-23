// Implementation

#include "PostgreSQL.h"

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
