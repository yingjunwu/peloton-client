#include "client_config.h"


void Usage(FILE *out) {
  fprintf(out,
          "Command line options : client_program <options> \n"
          "   -h --help              :  print help message \n"
          "   -t --thread_count      :  # of threads \n"
          "   -k --scale_factor      :  scale factor \n"
          "   -z --zipf_theta        :  zipf theta \n"
          "   -o --operation_count   :  # of operations \n"
          "   -u --update_ratio      :  update ratio \n"
          "   -i --with_index        :  with index \n"
          "   -p --with_prep_stmt    :  with prepared statement \n"
          "   -y --program_type      :  program type (populate/client/procedure/scan) \n"
  );
}

static struct option opts[] = {
    { "thread_count", optional_argument, NULL, 't' },
    { "scale_factor", optional_argument, NULL, 'k' },
    { "zipf_theta", optional_argument, NULL, 'z' },
    { "operation_count", optional_argument, NULL, 'o' },
    { "update_ratio", optional_argument, NULL, 'u' },
    { "with_index", no_argument, NULL, 'i' },
    { "with_prep_stmt", no_argument, NULL, 'p' },
    { "program_type", optional_argument, NULL, 'y' },
    { NULL, 0, NULL, 0 }
};

void ParseArguments(int argc, char *argv[], ClientConfig &conf) {

  conf.thread_count_ = 1;
  conf.scale_factor_ = 1;
  conf.zipf_theta_ = 0;
  conf.operation_count_ = 1;
  conf.update_ratio_ = 0;
  conf.with_index_ = false;
  conf.with_prep_stmt_ = false;
  conf.program_type_ = ProgramType::INVALID;
  
  // parse args
  while (1) {
    int idx = 0;
    int c = getopt_long(argc, argv, "hipt:k:z:o:u:y:", opts, &idx);

    if (c == -1) break;

    switch(c) {

      case 'i':
        conf.with_index_ = true;
        break;
      case 'p':
        conf.with_prep_stmt_ = true;
        break;
      case 't':
        conf.thread_count_ = atoi(optarg);
        break;
      case 'k':
        conf.scale_factor_ = atof(optarg);
        break;
      case 'z':
        conf.zipf_theta_ = atof(optarg);
        break;
      case 'o':
        conf.operation_count_ = atoi(optarg);
        break;
      case 'u':
        conf.update_ratio_ = atof(optarg);
        break;
      case 'y': {
        char *program_type = optarg;
        if (strcmp(program_type, "populate") == 0) {
          conf.program_type_ = ProgramType::POPULATE;
        } else if (strcmp(program_type, "client") == 0) {
          conf.program_type_ = ProgramType::CLIENT;
        } else if (strcmp(program_type, "procedure") == 0) {
          conf.program_type_ = ProgramType::PROCEDURE;
        } else if (strcmp(program_type, "scan") == 0) {
          conf.program_type_ = ProgramType::SCAN;
        } else {
          exit(EXIT_FAILURE);
        }
        break;
      }
      case 'h':
      Usage(stderr);
      exit(EXIT_FAILURE);
      break;

      default:
      Usage(stderr);
      exit(EXIT_FAILURE);

    }
  }

  if (conf.program_type_ == ProgramType::INVALID) {
    Usage(stderr);
    exit(EXIT_FAILURE);
  }
}

