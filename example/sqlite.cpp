#include <iostream>

#include "factory.h"
// #include "log_armory/src/factory.h"
#include "querybuilder/query_builder.h"

int main() {

    // -----------------------------
    // LOGGER CONFIG
    // -----------------------------
    // LogConfig lcfg;
    // lcfg.general_config.fileName = ".";
    // lcfg.rotate_config.max_count = 100;
    // lcfg.general_config.logLevel = LogLevel::info;

    // auto logger = LoggerFactory::createLogger(LoggerType::Spdlog, lcfg);

    // -----------------------------
    // SQLITE CONFIG
    // -----------------------------
    ConnectionConfig sqlite_cfg;
    sqlite_cfg.path = "mydb.db";

    // logger->info("Opening SQLite connection...");
    std::unique_ptr<IDatabase> sq =
        DatabaseFactory::createDatabase(DatabaseType::sqlite, sqlite_cfg
            // , std::move(logger)
        );

    std::cout << "SQLite open result: " << sq->open() << std::endl;

    // -----------------------------
    // SIMPLE SELECT
    // -----------------------------
    QueryBuilder q_;
    std::string sql_ = q_.table("users u")
                           .select("u.id")
                           .select("u.name")
                           .select("u.family")
                           .select("u.email")
                           .select("u.age")
                           .orderBy("u.id DESC")
                           .limit(10)
                           .offset(0)
                           .str();

    std::cout << "SQLite SELECT: " << sql_ << "\n";
    // sq->select(q_).print();

    // -----------------------------
    // INSERT
    // -----------------------------
    QueryBuilder qb_insert;
    qb_insert.insert("users").values(
        {{"name", "pouria"}, {"email", "mohammadi@IsIran.com"}, {"family", "Mohammadi"}, {"age", "22"}});
    sq->insert(qb_insert);

    // -----------------------------
    // UPDATE
    // -----------------------------
    QueryBuilder qb_update;
    qb_update.update("users").set("email", "Rezaei@IsIran.ir").where("id = 2");
    sq->update(qb_update);

    // -----------------------------
    // DELETE
    // -----------------------------
    QueryBuilder qb_delete;
    qb_delete.remove("users").where("id = 6");
    sq->remove(qb_delete);

    // -----------------------------
    // SHOW ALL
    // -----------------------------
    QueryBuilder show_all;
    show_all.table("users").select("*");
    sq->select(show_all).print();

    // -----------------------------
    // ✅ SQLITE INNER JOIN
    // -----------------------------
    QueryBuilder q_join;
    std::string sql_join = q_join.table("users u")
                               .select("u.id")
                               .select("u.name")
                               .select("u1.family")
                               .join("users u1", "u.id", "u1.id")   // INNER JOIN
                               .where("u.age > 18")
                               .orderBy("u.id DESC")
                               .limit(10)
                               .str();

    std::cout << "SQLite INNER JOIN: " << sql_join << "\n";
    sq->select(q_join).print();

    // -----------------------------
    // ✅ SQLITE LEFT JOIN
    // -----------------------------
    QueryBuilder q_left;
    std::string sql_left =
        q_left.table("users u")
             .select("u.id")
             .select("u.name")
             .select("u2.email")
             .leftJoin("users u2", "u.id", "u2.id")
             .str();

    std::cout << "SQLite LEFT JOIN: " << sql_left << "\n";
    sq->select(q_left).print();

    // -----------------------------
    // ✅ SQLITE MULTI JOIN
    // -----------------------------
    QueryBuilder q_multi;
    std::string sql_multi = q_multi.table("users u")
                                .select("u.id")
                                .select("u.name")
                                .join("users u3", "u.id", "u3.id")
                                .join("users u4", "u.id", "u4.id")
                                .where("u.age > 20")
                                .str();

    std::cout << "SQLite MULTI JOIN: " << sql_multi << "\n";
    sq->select(q_multi).print();

    return 0;
}
