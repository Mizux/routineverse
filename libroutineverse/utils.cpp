#include "utils.hpp"

#include <algorithm>
#include <random>

std::mt19937& rng() {
  static thread_local std::mt19937 gen(std::random_device{}());
  return gen;
}

int rand_int(int min_v, int max_v) {
  if (max_v <= min_v) return min_v;
  std::uniform_int_distribution<int> dist(min_v, max_v);
  return dist(rng());
}

std::string money_string(uint64_t value) { return number_string(value) + " Cr"; }

std::string number_string(uint64_t value) {
  std::string raw = std::to_string(value);
  std::string out;
  int count = 0;
  for (auto it = raw.rbegin(); it != raw.rend(); ++it) {
    if (count > 0 && count % 3 == 0) out.push_back(',');
    out.push_back(*it);
    ++count;
  }
  std::reverse(out.begin(), out.end());
  return out;
}

