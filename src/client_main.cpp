#include <iostream>
#include <cstdio>
#include <cassert>

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */

#include "fast_random.h"


FastRandom fast_rand;

size_t table_size = 1000;

void Populate(pqxx::work &txn_handle, const size_t table_size) {
  for (size_t i = 0; i < table_size; ++i) {
    txn_handle.exec("INSERT INTO employee VALUES (" + std::to_string(i) + ", 'a');");
  }
}

void Select(pqxx::connection &conn, pqxx::work &txn_handle, const size_t num_tuple, bool with_prepare) {
  if (with_prepare == true) {
    conn.prepare("read", "SELECT name FROM employee WHERE id=$1");
    for (size_t i = 0; i < num_tuple; ++i) {
      pqxx::result R = txn_handle.prepared("read")(fast_rand.next() % table_size).exec();
      printf("txn result set size = %lu\n", R.size());
    }
  } else {
    for (size_t i = 0; i < num_tuple; ++i) {
      pqxx::result R = txn_handle.exec("SELECT name FROM employee WHERE id=" + std::to_string(fast_rand.next() % table_size) + ";");
      printf("txn result set size = %lu\n", R.size());
    }
  }
}

void Update(pqxx::connection &conn, pqxx::work &txn_handle, const size_t num_tuple, bool with_prepare) {
  if (with_prepare == true) {
    conn.prepare("write", "UPDATE employee SET name = 'z' WHERE id=$1");
    for (size_t i = 0; i < num_tuple; ++i) {
      txn_handle.prepared("write")(fast_rand.next() % table_size).exec();
    }
  } else {
    for (size_t i = 0; i < num_tuple; ++i) {
      txn_handle.exec("UPDATE employee SET name = 'z' WHERE id=" + std::to_string(fast_rand.next() % table_size) + ";");
    }
  }
}

void Mix(pqxx::connection &conn, pqxx::work &txn_handle, const size_t num_tuple, const float read_ratio, bool with_prepare) {
  if (with_prepare == true) {
    conn.prepare("read", "SELECT name FROM employee WHERE id=$1");
    conn.prepare("write", "UPDATE employee SET name = 'z' WHERE id=$1");
    for (size_t i = 0; i < num_tuple; ++i) {
      if (fast_rand.next_uniform() < read_ratio) {
        // select
        pqxx::result R = txn_handle.prepared("read")(fast_rand.next() % table_size).exec();
        printf("txn result set size = %lu\n", R.size());
      } else {
        // update
        txn_handle.prepared("write")(fast_rand.next() % table_size).exec();
      }
    }
  } else {
    for (size_t i = 0; i < num_tuple; ++i) {
      if (fast_rand.next_uniform() < read_ratio) {
        // select
        pqxx::result R = txn_handle.exec("SELECT name FROM employee WHERE id=" + std::to_string(fast_rand.next() % table_size) + ";");
        printf("txn result set size = %lu\n", R.size());      
      } else {
        // update
        txn_handle.exec("UPDATE employee SET name = 'z' WHERE id=" + std::to_string(fast_rand.next() % table_size) + ";");
      }
    }
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

  if (argc != 2 && argc != 4 && argc != 5 && argc != 6) {
    printf("Usage: %s populate table_size with_index\n", argv[0]);
    printf("Usage: %s select table_size num_tuple with_prepare\n", argv[0]);
    printf("Usage: %s update table_size num_tuple with_prepare\n", argv[0]);
    printf("Usage: %s mix table_size num_tuple read_ratio with_prepare\n", argv[0]);
    printf("Usage: %s scan\n", argv[0]);
    return -1;
  }

  try {
    pqxx::connection C(
      "host=dev2.db.pdl.cmu.local port=5432 user=postgres sslmode=disable"
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

      assert(argc == 5);

      table_size  = atoi(argv[2]);
      size_t num_tuple = atoi(argv[3]);
      bool with_prepare = bool(atoi(argv[4]));

      printf("=========SELECT==========\n");
      pqxx::work txn(C);
      Select(C, txn, num_tuple, with_prepare);
      txn.commit();
    
    } else if (std::string(argv[1]) == "update") {

      assert(argc == 5);

      table_size  = atoi(argv[2]);
      size_t num_tuple = atoi(argv[3]);
      bool with_prepare = bool(atoi(argv[4]));

      printf("=========UPDATE==========\n");
      pqxx::work txn(C);
      Update(C, txn, num_tuple, with_prepare);
      txn.commit();

    } else if (std::string(argv[1]) == "mix") {

      assert(argc == 6);

      table_size = atoi(argv[2]);
      size_t num_tuple = atoi(argv[3]);
      float read_ratio = atof(argv[4]);
      bool with_prepare = bool(atoi(argv[4]));

      printf("=========MIX==========\n");
      pqxx::work txn(C);
      Mix(C, txn, num_tuple, read_ratio, with_prepare);
      txn.commit();

    } else if (std::string(argv[1]) == "scan") {

      printf("=========SCAN==========\n");
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
