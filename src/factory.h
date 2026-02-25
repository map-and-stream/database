#pragma once

#include "config.h"
#include "database.h"
#include "log_armory/src/logger.h"
#include "sqlite/sqlite.h"
#include <memory>
#ifdef USE_POSTGRESQL
#include "postgres/postgresql.h"
#endif

class DatabaseFactory {
  public:
    static std::unique_ptr<IDatabase> createDatabase(DatabaseType type, ConnectionConfig cfg,
                                                     std::unique_ptr<ILogger> logger) {
        if (type == DatabaseType::PostgreSQL) {
#ifdef USE_POSTGRESQL
            return std::make_unique<PostgreSQL>(cfg, std::move(logger));
#else
            throw std::invalid_argument("PostgreSQL support not built in this configuration");
#endif
        } else if (type == DatabaseType::sqlite) {
            return std::make_unique<SQLite>(cfg, std::move(logger));
        } else {
            throw std::invalid_argument("Invalid logger type");
        }
    }
};
