#pragma once

#include "postgresql.h"
#include "sqlite.h"
#include "config.h"

class DatabaseFactory {
  public:
    static std::unique_ptr<IDatabase> createDatabase(DatabaseType type, ConnectionConfig cfg) {
        if (type == DatabaseType::PostgreSQL) {
            return std::make_unique<PostgreSQL>(cfg);
        } else if (type == DatabaseType::sqlite) {
            return std::make_unique<SQLite>(cfg);
        }
        else {
            throw std::invalid_argument("Invalid logger type");
        }
    }
};
