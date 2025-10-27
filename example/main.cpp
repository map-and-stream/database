#include <iostream>

#include "config.h"
#include "postgresql.h"

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
    PostgreSQL pg(cfg.toPostgresConnection());  // TODO: use this way for create connection
    std::cout << "result of open :" << pg.open() << std::endl;
    // refacot paramter table, parameter query, parameter value
    // create test table
    // pg.insert("insert INTO users(username,password, city, email) values ($1, $2, $3, $4)",
    // std::vector<std::string>{"javad", "javadi","sadsad", "sdfdsfdsf"});

    pg.printResult(pg.select(std::string("select * from users;")));
    bool result = pg.update("UPDATE users SET name = $1 WHERE age = $2", {"Abolfazl Jr.", "19"});
    std::cout << "result of update :" << result << std::endl;
    result = pg.remove("DELETE FROM users WHERE age = $1", {"2"});
    std::cout << "result of delete :" << result << std::endl;

    return 0;
}
