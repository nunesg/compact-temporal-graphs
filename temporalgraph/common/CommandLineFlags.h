#include "gflags/gflags.h"

DEFINE_int32(has_edge_epochs, 10,
             "Number of times to run the has_edge operation");
DEFINE_int32(neighbours_epochs, 10,
             "Number of times to run the neighbours operation");
DEFINE_int32(aggregate_epochs, 10,
             "Number of times to run the aggregate operation");
DEFINE_string(
    output_file, "results.json",
    "Name of the file that will contain the result of the experiment");