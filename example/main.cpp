#include <iostream>

#include "config.h"
#include "postgresql.h"

int main() {
    ConnectionConfig cfg;
    cfg.host = "192.168.10.137";
    cfg.port = 5432;
    cfg.dbname = "postgres"; //users(id, username,password, city, email)
    cfg.user = "javadshekarian";
    cfg.password = "Isiran@123";
    cfg.connect_timeout = 5;

    std::cout << "try to open connection..." << std::endl;
    // PostgreSQL pg("postgresql://postgres:qazwsx@172.21.144.1:5432/mydb?connect_timeout=2");
    PostgreSQL pg(cfg.toPostgresConnection()); //TODO: use this way for create connection
    std::cout << "result of open :" << pg.open() << std::endl;
// refacot paramter table, parameter query, parameter value
//create test table
    // pg.insert("insert INTO users(username,password, city, email) values ($1, $2, $3, $4)", std::vector<std::string>{"javad", "javadi","sadsad", "sdfdsfdsf"});

    pg.printResult(pg.select(std::string("select * from users;")));

    return 0;
}
