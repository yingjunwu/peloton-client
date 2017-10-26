#pragma once

#include <getopt.h>

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>

enum class ProgramType {
  POPULATE,
  PROCESS,
  SCAN,
  INVALID,
};


struct ClientConfig {

  const size_t default_table_size_ = 1000;
  
  size_t thread_count_;
  
  float scale_factor_;
  
  float zipf_theta_;
  
  // operation count in a transaction
  size_t operation_count_;

  // update ratio
  float update_ratio_;

  // populate table
  bool is_populate_;

  // build_index
  bool with_index_;

  // with prepared statement
  bool with_prep_stmt_;

  ProgramType program_type_;

};

void Usage(FILE *out);

void ParseArguments(int argc, char *argv[], ClientConfig &);
