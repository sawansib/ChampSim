#ifndef CLI_H
#define CLI_H

#include <cstdint>
#include <string_view>
#include <tuple>
#include <vector>

std::tuple<bool, bool, uint64_t, uint64_t, std::vector<std::string_view>> parse_args(int argc, char** argv);

#endif

