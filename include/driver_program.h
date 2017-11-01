#pragma once

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */

#include "driver_config.h"

void Populate(pqxx::connection &conn, const DriverConfig &config);

void ProcessClient(pqxx::connection &conn, const DriverConfig &config);

void ProcessProcedure(pqxx::connection &conn, const DriverConfig &config);

void Scan(pqxx::connection &conn);

