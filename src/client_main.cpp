#include <cstdio>

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */


int main(int argc, char **argv) {

  try {
    pqxx::connection C(
        "host=127.0.0.1 port=5431 user=postgres sslmode=disable"
    );
    printf("Connected to %s\n", C.dbname());
    pqxx::work txn1(C);

    txn1.exec("DROP TABLE IF EXISTS employee;");
    txn1.exec("CREATE TABLE employee(id INT, name VARCHAR(100));");
    txn1.commit();

    pqxx::work txn2(C);
    txn2.exec("INSERT INTO employee VALUES (1, 'a');");
    txn2.exec("INSERT INTO employee VALUES (2, 'b');");
    txn2.exec("INSERT INTO employee VALUES (3, 'c');");

    pqxx::result R = txn2.exec("SELECT name FROM employee where id=1;");
    txn2.commit();
    
    printf("txn2 result set size = %lu\n", R.size());

  } catch (const std::exception &e) {
    printf("Exception occurred: %s\n", e.what());
    exit(-1);
  }

  return 0;
}
