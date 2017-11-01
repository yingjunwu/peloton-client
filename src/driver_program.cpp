#include <iostream>
#include <cstdio>
#include <cassert>

#include "driver_program.h"
#include "fast_random.h"

void Populate(pqxx::connection &conn, const DriverConfig &config) {

  size_t table_size = config.default_table_size_ * config.scale_factor_;
  
  std::cout << ">>>>> Populate table \'employee\'. " << std::endl
            << "   -- Build index? : " << config.with_index_ << std::endl
            << "   -- Table size   : " << table_size << std::endl;

  pqxx::work txn(conn);

  txn.exec("DROP TABLE IF EXISTS employee;");
  txn.exec("CREATE TABLE employee(id INT, name VARCHAR(100));");
  
  if (config.with_index_ == true) {
    txn.exec("CREATE INDEX emp_index ON employee(id)");
  }

  for (size_t i = 0; i < table_size; ++i) {
    txn.exec("INSERT INTO employee VALUES (" + std::to_string(i) + ", 'a');");
  }
  txn.commit();
}

void ProcessClient(pqxx::connection &conn, const DriverConfig &config) {

  size_t table_size = config.default_table_size_ * config.scale_factor_;

  FastRandom fast_rand;

  ZipfDistribution zipf(table_size, config.zipf_theta_);

  std::cout << ">>>>> Process transactions via client interface. " << std::endl
            << "   -- With prepared statement? : " << config.with_prep_stmt_ << std::endl
            << "   -- Table size               : " << table_size << std::endl
            << "   -- Operation count          : " << config.operation_count_ << std::endl
            << "   -- Update ratio             : " << config.update_ratio_ << std::endl
            << "   -- Zipf theta               : " << config.zipf_theta_ << std::endl;
  
  pqxx::work txn(conn);

  if (config.with_prep_stmt_ == true) {

    conn.prepare("read", "SELECT name FROM employee WHERE id=$1");
    conn.prepare("write", "UPDATE employee SET name = 'z' WHERE id=$1");
    
    for (size_t i = 0; i < config.operation_count_; ++i) {
      
      size_t key = zipf.GetNextNumber() - 1;
      
      if (fast_rand.next_uniform() < config.update_ratio_) {
        // update
        txn.prepared("write")(key).exec();
      } else {
        // select
        pqxx::result R = txn.prepared("read")(key).exec();
        printf("key = %lu, txn result set size = %lu\n", key, R.size());
      }
    }
  } else {
    for (size_t i = 0; i < config.operation_count_; ++i) {
      
      size_t key = zipf.GetNextNumber() - 1;
      
      if (fast_rand.next_uniform() < config.update_ratio_) {
        // update
        txn.exec("UPDATE employee SET name = 'z' WHERE id=" + std::to_string(key) + ";");
      } else {
        // select
        pqxx::result R = txn.exec("SELECT name FROM employee WHERE id=" + std::to_string(key) + ";");
        printf("key = %lu, txn result set size = %lu\n", key, R.size());
      }
    }
  }
  txn.commit();
}

void ProcessProcedure(pqxx::connection &conn, const DriverConfig &config) {

  size_t table_size = config.default_table_size_ * config.scale_factor_;

  std::cout << ">>>>> Process transactions via stored procedure. "
            << "   -- Table size      : " << table_size << std::endl
            << "   -- Operation count : " << config.operation_count_ << std::endl
            << "   -- Update ratio    : " << config.update_ratio_ << std::endl
            << "   -- Zipf theta      : " << config.zipf_theta_ << std::endl;

  FastRandom fast_rand;

  ZipfDistribution zipf(table_size, config.zipf_theta_);

  pqxx::nontransaction nontxn(conn);

  // wrong procedure!  
  std::string func("CREATE OR REPLACE FUNCTION inc(val integer) RETURNS integer AS $$ \
              BEGIN \
              RETURN SELECT b FROM test where a = 1; \
              END; $$ \
              LANGUAGE PLPGSQL;");

  nontxn.exec(func.c_str());

}

void Scan(pqxx::connection &conn) {
  
  pqxx::work txn(conn);

  std::cout << ">>>>> Scan table." << std::endl;

  pqxx::result R = txn.exec("SELECT * FROM employee;");
  
  printf("txn result set size = %lu\n", R.size());
  
  for (size_t i = 0; i < R.size(); ++i) {
    int id = R[i][0].as<int>();
    std::string name = R[i][1].as<std::string>();
    std::cout << "id = " << id << ", " << name << std::endl;
  }
  
  txn.commit();
}
