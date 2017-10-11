#include <cstdio>
#include <cassert>

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */

void Populate(pqxx::work &txn_handle, const size_t table_size) {
  for (size_t i = 0; i < table_size; ++i) {
    txn_handle.exec("INSERT INTO employee VALUES (" + std::to_string(i) + ", 'a');");
  }
}


void Select(pqxx::work &txn_handle, const size_t num_tuple) {
  for (size_t i = 0; i < num_tuple; ++i) {
    pqxx::result R = txn_handle.exec("SELECT name FROM employee WHERE id=" + std::to_string(i) + ";");
    printf("txn result set size = %lu\n", R.size());
  }
}


int main(int argc, char **argv) {

  if (argc != 2 && argc != 3) {
    printf("please input arguments\n");
    return -1;
  }

  try {
    pqxx::connection C(
      "host=172.19.146.5 port=5432 user=postgres sslmode=disable"
    );
    printf("Connected to %s\n", C.dbname());

    if (std::string(argv[1]) == "populate") {
      
      size_t table_size = 100;

      if (argc != 2) {
        assert(argc == 3);
        table_size = atoi(argv[2]);
      }

      printf("populate table!\n");
      pqxx::work txn(C);
      txn.exec("DROP TABLE IF EXISTS employee;");
      txn.exec("CREATE TABLE employee(id INT, name VARCHAR(100));");
      Populate(txn, table_size);
      txn.commit();

    } else if (std::string(argv[1]) == "select") {

      size_t num_tuple = 1;

      if (argc != 2) {
        assert(argc == 3);
        num_tuple = atoi(argv[2]);
      }

      printf("select from table!\n");
      pqxx::work txn(C);
      Select(txn, num_tuple);
      txn.commit();
    } else {
      printf("please input correct arguments \n");
      return -1;
    }
    
  } catch (const std::exception &e) {
    printf("Exception occurred: %s\n", e.what());
    exit(-1);
  }

  return 0;
}
