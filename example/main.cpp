#include "factory.h"
#include <iostream>


int main() {
  try {
    // e.g., runtime selection based on config or environment
    DatabaseType type = DatabaseType::SQLite;

    auto db = DatabaseFactory::create(type, argc, argv);

    std::cout << "Database created successfully!\n";
  } catch (const std::exception& e) {
    std::cerr << "Error creating database: " << e.what() << '\n';
  }

  auto db = DatabaseFactory::create(DatabaseType::SQLite, argc, argv);

  OdbCrudRepository<Person> repo(*db);
  OdbQueryBuilder<Person> qb(*db);

  // Create
  Person p("Alice", 25);
  repo.create(p);

  // Read
  auto loaded = repo.read("1");
  if (loaded) std::cout << loaded->name() << "\n";

  // Update
  loaded->set_age(26);
  repo.update(*loaded);

  // Delete
  repo.remove("1");

  // Query
  auto results = qb.where("name", "Bob").execute();
  for (auto& r : results)
    std::cout << r->name() << "\n";

  return 0;
}
