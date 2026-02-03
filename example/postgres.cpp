#include <iostream>

#include "factory.h"
#include "log_armory/src/factory.h"
#include "log_armory/src/logger.h"
#include "querybuilder/query_builder.h"

int main() {
    /*
     * Schema for table 'users':
     * id     : integer, primary key, auto-increment
     * name   : text or varchar, not null
     * family : text or varchar, not null
     * age    : integer
     * email  : text or varchar, unique, not null
     * Example: users(id, name, family, age, email)
     */

    ConnectionConfig cfg;
    cfg.host = "192.168.10.127";
    cfg.port = 5432;
    cfg.dbname = "mydb";
    cfg.user = "postgres";
    cfg.password = "qazwsx";
    cfg.connect_timeout = 5;

    LogConfig lcfg;
    lcfg.general_config.fileName = ".";
    lcfg.rotate_config.max_count = 100;
    lcfg.general_config.logLevel = LogLevel::info;
    auto logger = LoggerFactory::createLogger(LoggerType::Spdlog, lcfg);
    ILogger* log = logger.get();

    std::unique_ptr<IDatabase> pg;
    {
        log->info("try to open connection...");
        pg = DatabaseFactory::createDatabase(DatabaseType::PostgreSQL, cfg, std::move(logger));

        if (!pg) {
            log->error("failed to create postgres database object");
            return 1;
        }

        if (!pg->open()) {
            log->error("postgres connection failed");
            return 1;
        }

        log->info("postgres connection is open");
    }

    {
        QueryBuilder query_select;
        std::string sql = query_select.table("users u")
                              .select("u.id")
                              .select("u.name")
                              .select("u.famile")
                              .select("u.email")
                              .select("u.age")
                              .orderBy("u.id DESC")
                              .limit(10)
                              .offset(0)
                              .str();
        log->info("query of querybuilder :" + sql);
    }

    {
        QueryBuilder qb_insert;
        qb_insert.insert("users").values(
            {{"name", "Qasem"}, {"email", "moradi@IsIran.com"}, {"famile", "moradi"}, {"age", "35"}});
        std::string insert_sql = qb_insert.str();
        std::cout << "INSERT query: " << insert_sql << "\n";
        bool insert_result = pg->insert(qb_insert);
        std::cout << "Result of insert: " << insert_result << std::endl;
    }

    {
        QueryBuilder qb_update;
        qb_update.update("users").set("email", "kermatIsIran@isIran.ir").where("id = 26");
        bool update_result = pg->update(qb_update);
        std::cout << "Result of update: " << update_result << std::endl;
    }

    {
        QueryBuilder qb_delete;
        qb_delete.remove("users").where("id = 9");
        std::string delete_sql = qb_delete.str();
        std::cout << "DELETE query: " << delete_sql << "\n";
        // Execute DELETE
        bool delete_result = pg->remove(qb_delete);
        std::cout << "Result of delete: " << delete_result << std::endl;
    }

    {
        QueryBuilder show_all;
        show_all.table("users").select("*");
        pg->select(show_all).print();
    }

    QueryBuilder q_join;
    std::string sql_join = q_join.table("users u")
                               .select("u.id")
                               .select("u.name")
                               .join("users u1", "u.id", "u1.id")  // INNER JOIN
                               .where("u.age > 18")
                               .orderBy("u.id DESC")
                               .limit(10)
                               .str();

    std::cout << "JOIN query: " << sql_join << "\n";

    // Execute
    pg->select(q_join).print();

    QueryBuilder q_left;
    std::string sql_left =
        q_left.table("users u").select("u.id").select("u.name").leftJoin("users u2", "u.id", "u2.id").str();

    std::cout << "LEFT JOIN query: " << sql_left << "\n";
    pg->select(q_left).print();

    QueryBuilder q_multi;
    std::string sql_multi = q_multi.table("users u")
                                .select("u.id")
                                .select("u.name")
                                .join("users u3", "u.id", "u3.id")
                                .join("users u4", "u.id", "u4.id")
                                .where("u.age > 20")
                                .str();

    std::cout << "MULTI JOIN query: " << sql_multi << "\n";
    pg->select(q_multi).print();
    return 0;
}
