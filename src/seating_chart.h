#ifndef SEATING_CHART_H
#define SEATING_CHART_H

#include <algorithm>
#include <array>
#include <ranges>
#include <utility>

#include "6x8.h"

using student_t = short;
using location_t = std::pair<short, short>;

struct seating_chart {
  std::array<student_t, s_num> chart; // (row, col) -> student
  std::array<short, s_num> inv_chart; // student -> (row, col)

  seating_chart(std::array<student_t, s_num> c) noexcept;

  double score() const noexcept;
  void export_c() const noexcept;

  void swap(short i, short j) noexcept;
  void swap_table(short i, short j) noexcept;

  bool hill_climb() noexcept;

  template <typename G>
    requires std::uniform_random_bit_generator<G>
  void shuffle(G &g) noexcept;

  template <typename G>
    requires std::uniform_random_bit_generator<G>
  void partial_shuffle(G &g) noexcept;
};

template <typename G>
  requires std::uniform_random_bit_generator<G>
void seating_chart::shuffle(G &g) noexcept {
  std::ranges::shuffle(chart, g);
  for (const auto i : std::views::iota(0, s_num))
    inv_chart[chart[i]] = i;
}

constexpr auto swap_n = 12;
template <typename G>
  requires std::uniform_random_bit_generator<G>
void seating_chart::partial_shuffle(G &g) noexcept {
  auto dist = std::uniform_int_distribution<short>{0, s_num - 1};
  auto flip = std::uniform_int_distribution<short>{0, 1};

  if (flip(g))
    for (const auto i : std::views::iota(0, swap_n))
      swap(inv_chart[i], inv_chart[dist(g)]);
  else
    for (const auto i : std::views::iota(s_num - swap_n, s_num - 1))
      swap(inv_chart[i], inv_chart[dist(g)]);
}

#endif
