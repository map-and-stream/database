#include <iostream>

#include "config.h"
#include "postgresql.h"

int main() {
    ConnectionConfig cfg;
    cfg.host = "localhost";
    cfg.port = 5432;
    cfg.dbname = "mydb";
    cfg.user = "myuser";
    cfg.password = "mypassword";
    cfg.connect_timeout = 5;

    std::cout << "try to open connection..." << std::endl;
    PostgreSQL pg("postgresql://postgres:qazwsx@172.21.144.1:5432/mydb?connect_timeout=2");
    // PostgreSQL pg(cfg.toPostgresConnection()); //TODO: use this way for create connection
    std::cout << "result of open :" << pg.open() << std::endl;

    return 0;
}
