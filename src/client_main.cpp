#include <iostream>
#include <cassert>

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */

#include "client_program.h"

ClientConfig config;

int main(int argc, char **argv) {

  ParseArguments(argc, argv, config);

  try {
    pqxx::connection conn(
      "host=dev2.db.pdl.cmu.local port=5432 user=postgres sslmode=disable"
    );
    printf("Connected to %s\n", conn.dbname());

    if (config.program_type_ == ProgramType::POPULATE) {
      // populate table
      Populate(conn, config);
    
    } else if (config.program_type_ == ProgramType::CLIENT) {
      // process transactions via client interface
      ProcessClient(conn, config);
    
    } else if (config.program_type_ == ProgramType::PROCEDURE) {
      // process transactions via stored procedure
      ProcessProcedure(conn, config);

    } else if (config.program_type_ == ProgramType::SCAN) {
      // scan table
      Scan(conn);
    
    } else {
      exit(EXIT_FAILURE);
    }
    
  } catch (const std::exception &e) {
    printf("Exception occurred: %s\n", e.what());
    exit(EXIT_FAILURE);
  }

  return 0;
}
