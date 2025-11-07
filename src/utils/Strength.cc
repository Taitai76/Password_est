#include "Strength.h"
#include <cctype>
#include <unordered_set>
#include <algorithm>
#include <cmath>

static double entropy_guess(size_t len, bool lower, bool upper, bool digit, bool symbol) {
  double charset = 0;
  if (lower)  charset += 26;
  if (upper)  charset += 26;
  if (digit)  charset += 10;
  if (symbol) charset += 33; // rough ASCII punctuation set
  if (charset < 1) charset = 1;
  return len * std::log2(charset);
}

StrengthResult estimate_strength(const std::string& pw) {
  StrengthResult r{};
  std::vector<std::string> fb;

  if (pw.empty()) {
    r.score = 0; r.verdict = "very weak"; r.entropy_bits = 0.0;
    fb.push_back("Password is empty.");
    r.feedback = fb; return r;
  }

  bool hasLower=false, hasUpper=false, hasDigit=false, hasSymbol=false;
  for (unsigned char c : pw) {
    if (std::islower(c)) hasLower = true;
    else if (std::isupper(c)) hasUpper = true;
    else if (std::isdigit(c)) hasDigit = true;
    else hasSymbol = true;
  }

  // naive pattern penalties
  auto is_sequence = [](const std::string& s){
    int run = 1;
    for (size_t i=1;i<s.size();++i){
      if (static_cast<int>(s[i]) - static_cast<int>(s[i-1]) == 1) run++;
      else run = 1;
      if (run >= 4) return true;
    }
    return false;
  };

  auto repeats = [](const std::string& s){
    for (size_t i=1;i<s.size();++i) if (s[i]==s[i-1]) return true;
    return false;
  };

  static const std::unordered_set<std::string> common = {
    "password","123456","qwerty","letmein","admin","welcome","iloveyou"
  };

  std::string lower = pw;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  int points = 0;
  if (pw.size() >= 8) points++;
  if (pw.size() >= 12) points++;
  if ((hasLower + hasUpper + hasDigit + hasSymbol) >= 2) points++;
  if ((hasLower + hasUpper + hasDigit + hasSymbol) >= 3) points++;
  if ((hasLower + hasUpper + hasDigit + hasSymbol) == 4) points++;

  if (is_sequence(lower)) { points = std::max(0, points-1); fb.push_back("Avoid sequential patterns (e.g., abcd, 1234)."); }
  if (repeats(lower))     { fb.push_back("Avoid repeated characters."); }
  if (common.count(lower)){ points = 0; fb.push_back("This password is too common."); }

  r.entropy_bits = entropy_guess(pw.size(), hasLower, hasUpper, hasDigit, hasSymbol);

  // map points (0..5) to score 0..4
  r.score = std::clamp(points - 1, 0, 4);
  static const char* words[] = {"very weak","weak","fair","good","strong"};
  r.verdict = words[r.score];

  if (pw.size() < 12) fb.push_back("Use at least 12 characters.");
  if ((hasLower + hasUpper + hasDigit + hasSymbol) < 3) fb.push_back("Mix upper/lowercase, digits, and symbols.");

  r.feedback = fb;
  return r;
}
