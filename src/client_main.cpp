#include <iostream>
#include <cstdio>
#include <cassert>

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */

#include "fast_random.h"


FastRandom fast_rand;

size_t table_size = 100;

void Populate(pqxx::work &txn_handle, const size_t table_size) {
  for (size_t i = 0; i < table_size; ++i) {
    txn_handle.exec("INSERT INTO employee VALUES (" + std::to_string(i) + ", 'a');");
  }
}


void Select(pqxx::work &txn_handle, const size_t num_tuple) {
  for (size_t i = 0; i < num_tuple; ++i) {
    pqxx::result R = txn_handle.exec("SELECT name FROM employee WHERE id=" + std::to_string(fast_rand.next() % table_size) + ";");
    printf("txn result set size = %lu\n", R.size());
  }
}


void Update(pqxx::work &txn_handle, const size_t num_tuple) {
  for (size_t i = 0; i < num_tuple; ++i) {
    txn_handle.exec("UPDATE employee SET name = 'z' WHERE id=" + std::to_string(fast_rand.next() % table_size) + ";");
  }
}

void Scan(pqxx::work &txn_handle) {
  pqxx::result R = txn_handle.exec("SELECT * FROM employee;");
  printf("txn result set size = %lu\n", R.size());
  for (size_t i = 0; i < R.size(); ++i) {
    int id = R[i][0].as<int>();
    std::string name = R[i][1].as<std::string>();
    std::cout << "id = " << id << ", " << name << std::endl;
  }
}

int main(int argc, char **argv) {

  if (argc != 2 && argc != 3 && argc != 4) {
    printf("Usage: %s populate table_size with_index\n", argv[0]);
    printf("Usage: %s select table_size num_tuple\n", argv[0]);
    printf("Usage: %s update table_size num_tuple\n", argv[0]);
    return -1;
  }

  try {
    pqxx::connection C(
      "host=172.19.146.5 port=5432 user=postgres sslmode=disable"
    );
    printf("Connected to %s\n", C.dbname());

    if (std::string(argv[1]) == "populate") {

      assert(argc == 4);
      
      table_size = atoi(argv[2]);
      bool with_index = bool(atoi(argv[3]));
      
      printf("populate table!\n");
      pqxx::work txn(C);
      txn.exec("DROP TABLE IF EXISTS employee;");
      txn.exec("CREATE TABLE employee(id INT, name VARCHAR(100));");
      
      if (with_index == true) {
        txn.exec("CREATE INDEX emp_index ON employee(id)");
      }

      Populate(txn, table_size);
      txn.commit();

    } else if (std::string(argv[1]) == "select") {

      assert(argc == 4);

      table_size  = atoi(argv[2]);
      size_t num_tuple = atoi(argv[3]);
      
      printf("select from table!\n");
      pqxx::work txn(C);
      Select(txn, num_tuple);
      txn.commit();
    
    } else if (std::string(argv[1]) == "update") {

      assert(argc == 4);

      table_size  = atoi(argv[2]);
      size_t num_tuple = atoi(argv[3]);

      printf("update table!\n");
      pqxx::work txn(C);
      Update(txn, num_tuple);
      txn.commit();

    } else if (std::string(argv[1]) == "scan") {

      printf("scan table!\n");
      pqxx::work txn(C);
      Scan(txn);
      txn.commit();

    } else {
      printf("please input correct arguments\n");
      return -1;
    }
    
  } catch (const std::exception &e) {
    printf("Exception occurred: %s\n", e.what());
    exit(-1);
  }

  return 0;
}
