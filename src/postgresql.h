#include <iostream>
#include <memory>
#include <pqxx/pqxx>  // This includes the full definition of pqxx::connection
#include <string>
#include <vector>

class PostgreSQL {
  public:
    PostgreSQL(const std::string& conninfo) : connection_(nullptr), conninfo_(conninfo) {}

    bool open();
    void close();
    bool is_open() const;
    pqxx::connection* get();

    bool insert(const std::string& query, const std::vector<std::string>& values);
    pqxx::result select(const std::string& query, const std::vector<std::string>& params = {});
    void printResult(const pqxx::result& res) {
        for (const auto& row : res) {
            for (const auto& field : row) {
                std::cout << field.c_str() << "\t";  // print column value
            }
            std::cout << "\n";
        }
    }
    bool update(const std::string& query, const std::vector<std::string>& params);
    bool remove(const std::string& query, const std::vector<std::string>& params);

    // Non-copyable
    PostgreSQL(const PostgreSQL&) = delete;
    PostgreSQL& operator=(const PostgreSQL&) = delete;

    // Movable
    PostgreSQL(PostgreSQL&&) = default;
    PostgreSQL& operator=(PostgreSQL&&) = default;

    ~PostgreSQL();

  private:
    std::unique_ptr<pqxx::connection> connection_;
    std::string conninfo_;
};