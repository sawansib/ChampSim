#include "cli.h"

#include <algorithm>
#include <charconv>
#include <getopt.h>
#include <iostream>
#include <string_view>
#include <system_error>

template <typename T>
T read_optarg(std::string_view arg)
{
  T retval;
  auto begin = std::data(arg);
  auto end   = begin + std::size(arg);
  auto [ptr, errc] = std::from_chars(begin, end, retval);

  if (errc == std::errc::invalid_argument || ptr != end) {
    std::cout << "Argument " << ptr << " is not a number." << std::endl;
    abort();
  } else if (errc == std::errc::result_out_of_range) {
    std::cout << "Argument " << ptr << " is too large." << std::endl;
    abort();
  }

  return retval;
}

std::tuple<bool, bool, uint64_t, uint64_t, std::vector<std::string_view>> parse_args(int argc, char** argv)
{
  uint64_t warmup_instructions = 1000000, simulation_instructions = 10000000;
  bool knob_hide_heartbeat = false;
  bool knob_cloudsuite = false;

  int traces_encountered = 0;
  static struct option long_options[] = {{"warmup_instructions", required_argument, 0, 'w'},
                                         {"simulation_instructions", required_argument, 0, 'i'},
                                         {"hide_heartbeat", no_argument, 0, 'h'},
                                         {"cloudsuite", no_argument, 0, 'c'},
                                         {"traces", no_argument, &traces_encountered, 1}, // Either --traces or -- terminates parsing
                                         {0, 0, 0, 0}};

  int c;
  while ((c = getopt_long_only(argc, argv, "w:i:hc", long_options, NULL)) != -1 && !traces_encountered) {
    switch (c) {
    case 'w':
      warmup_instructions = read_optarg<uint64_t>(optarg);
      break;
    case 'i':
      simulation_instructions = read_optarg<uint64_t>(optarg);
      break;
    case 'h':
      knob_hide_heartbeat = true;
      break;
    case 'c':
      knob_cloudsuite = true;
      break;
    case 0:
      break;
    default:
      abort();
    }
  }

  std::vector<std::string_view> trace_names{std::next(argv, optind), std::next(argv, argc)};

  return {knob_cloudsuite, knob_hide_heartbeat, warmup_instructions, simulation_instructions, trace_names};
}
