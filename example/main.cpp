#include <iostream>

#include "postgresql.h"

int main() {
    std::cout << "try to open connection..." << std::endl;
    PostgreSQL pg("postgresql://postgres:qazwsx@172.21.144.1:5432/mydb?connect_timeout=2");
    std::cout << "result of open :" << pg.open();

    return 0;
}
