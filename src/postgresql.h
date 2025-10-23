#include <memory>
#include <pqxx/pqxx>  // This includes the full definition of pqxx::connection
#include <string>

class PostgreSQL {
  public:
    PostgreSQL(const std::string& conninfo) : connection_(nullptr), conninfo_(conninfo) {}

    bool open();
    void close();
    bool is_open() const;
    pqxx::connection* get();

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