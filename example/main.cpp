#include <iostream>

#include "postgresql.h"

int main() {
    PostgreSQL pg("postgresql://postgres:qazwsx@172.21.144.1:5432/mydb");
    std::cout << "result of open :" << pg.open();

    return 0;
}
