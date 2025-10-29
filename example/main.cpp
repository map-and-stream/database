#include <iostream>

#include "factory.h"
#include "querybuilder/query_builder.h"

int main() {
    ConnectionConfig cfg;
    cfg.host = "172.21.144.1";
    cfg.port = 5432;
    cfg.dbname = "mydb";  // users(id, username,password, city, email)
    cfg.user = "postgres";
    cfg.password = "qazwsx";
    cfg.connect_timeout = 5;

    std::cout << "try to open connection..." << std::endl;
    // PostgreSQL pg("postgresql://postgres:qazwsx@172.21.144.1:5432/mydb?connect_timeout=2");
    std::unique_ptr<IDatabase> pg = DatabaseFactory::createDatabase(DatabaseType::PostgreSQL, cfg);

    // PostgreSQL pg(cfg);  // TODO: use this way for create connection
    std::cout << "result of open postgres :" << pg->open() << std::endl;
    QueryBuilder q;
    std::string sql = q.table("users u")
                          .select("u.id")
                          .select("u.name")
                          .select("u.email")
                          .orderBy("u.id DESC")
                          .limit(10)
                          .offset(0)
                          .str();

    std::cout << "query of querybuilder :" << sql << "\n";
    pg->select(q).print();

    // refacot paramter table, parameter query, parameter value
    // create test table
    // pg.insert("insert INTO users(username,password, city, email) values ($1, $2, $3, $4)",
    // std::vector<std::string>{"javad", "javadi","sadsad", "sdfdsfdsf"});

    // pg->select(std::string("select * from users;"));
    // bool result = pg->update("UPDATE users SET name = $1 WHERE age = $2", {"Abolfazl Jr.",
    // "19"}); std::cout << "result of update :" << result << std::endl; result = pg->remove("DELETE
    // FROM users WHERE age = $1", {"2"}); std::cout << "result of delete :" << result << std::endl;

    cfg.path = "mydb.db";
    std::unique_ptr<IDatabase> sq = DatabaseFactory::createDatabase(DatabaseType::sqlite, cfg);
    std::cout << "result of open sqlite :" << sq->open() << std::endl;

    return 0;
}
