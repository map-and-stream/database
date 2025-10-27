#pragma once

#include <iostream>
#include <string>

struct ConnectionConfig {
    std::string host;
    int port = 5432;
    std::string dbname;
    std::string user;
    std::string password;
    int connect_timeout = 10;  // seconds

    std::string toPostgresConnection() const {
        // postgresql://postgres:qazwsx@172.21.144.1:5432/mydb?connect_timeout=2
        // return "postgresql://" + user + ":" + password + "@" + host + ":" + std::to_string(port)
        // +
        //        "/" + dbname + "?" + "connect_timeout=" + std::to_string(connect_timeout);
        return "host=" + host + " port=" + std::to_string(port) + " dbname=" + dbname +
               " user=" + user + " password=" + password +
               " connect_timeout=" + std::to_string(connect_timeout);
    }
};