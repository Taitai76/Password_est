#pragma once
#include <string>
#include <vector>

struct StrengthResult {
  int score;                // 0-4
  std::string verdict;      // "very weak"..."strong"
  std::vector<std::string> feedback;
  double entropy_bits;      // rough estimate
};

StrengthResult estimate_strength(const std::string& pw);
