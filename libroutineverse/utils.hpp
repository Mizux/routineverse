#pragma once

#include <cstdint>
#include <random>
#include <string>

std::string money_string(uint64_t value);
std::string number_string(uint64_t value);

std::mt19937& rng();
int rand_int(int min_v, int max_v);
