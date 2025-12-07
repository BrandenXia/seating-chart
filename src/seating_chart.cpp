#include "seating_chart.h"

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <ranges>

#include "6x8.h"

inline constexpr location_t get_seat_position(short idx) {
  return {idx / col_num, idx % col_num};
}

inline constexpr std::array<location_t, s_num> idx2loc = [] {
  std::array<location_t, s_num> locs{};
  for (short i = 0; i < s_num; ++i)
    locs[i] = get_seat_position(i);
  return locs;
}();

inline double distance(const location_t &a, const location_t &b) {
  const auto dr = static_cast<double>(a.first - b.first);
  const auto dc = static_cast<double>(a.second - b.second);
  return std::sqrt(dr * dr + dc * dc);
}

seating_chart::seating_chart(std::array<student_t, s_num> c) noexcept
    : chart(std::move(c)) {
  for (short i = 0; i < s_num; ++i)
    inv_chart[chart[i]] = i;
}

inline auto transform_sum(auto &&iter, auto &&func) noexcept {
  return std::ranges::fold_left(iter | std::views::transform(func), 0,
                                std::plus<>());
}

inline constexpr short get_peer_idx(location_t loc) noexcept {
  const auto col = loc.second;
  const auto peer_col = col % 2 == 0 ? col + 1 : col - 1;
  return loc.first * col_num + peer_col;
}
inline constexpr short get_peer_idx(short idx) noexcept {
  return get_peer_idx(idx2loc[idx]);
}

double seating_chart::score() const noexcept {
  return transform_sum(chart, [&](const auto student) {
    const auto loc = idx2loc[inv_chart[student]];
    const auto &fs = friends[student];
    const auto f_score = transform_sum(fs, [&](const auto f) {
      const auto f_loc = idx2loc[inv_chart[f]];
      return 4 / std::pow(distance(loc, f_loc), 2);
    });
    const auto &es = enemies[student];
    const auto e_score = transform_sum(es, [&](const auto e) {
      const auto e_loc = idx2loc[inv_chart[e]];
      return 3 / std::pow(distance(loc, e_loc), 2);
    });

    const auto peer_idx = get_peer_idx(loc);
    const auto peer = chart[peer_idx];
    const auto g_score = fs.contains(peer) ? 5 : es.contains(peer) ? -5 : 0;

    return f_score - e_score + g_score;
  });
}

void seating_chart::export_c() const noexcept {
  namespace fs = std::filesystem;
  fs::create_directory("out");
  const auto path = fs::path("out") / std::format("{}.txt", score());
  std::ofstream ofs(path);

  for (const auto &s : chart)
    ofs << student_names[s] << '\n';
}

void seating_chart::swap(short i, short j) noexcept {
  auto s_i = chart[i], s_j = chart[j];
  chart[j] = s_i, chart[i] = s_j;
  inv_chart[s_i] = j, inv_chart[s_j] = i;
}

void seating_chart::swap_table(short i, short j) noexcept {
  swap(i, j);
  swap(get_peer_idx(i), get_peer_idx(j));
}

bool seating_chart::hill_climb() noexcept {
  auto max = score();
  auto max_pair = std::pair<short, short>{-1, -1};
  auto table_swap = false;

  for (const auto i : std::views::iota(0, s_num))
    for (const auto j : std::views::iota(i + 1, s_num)) {
      swap(i, j);
      const auto s1 = score();
      if (s1 > max) {
        max = s1;
        max_pair = {i, j};
        table_swap = false;
      }
      swap(i, j);

      swap_table(i, j);
      const auto s2 = score();
      if (s2 > max) {
        max = s2;
        max_pair = {i, j};
        table_swap = true;
      }
      swap_table(i, j);
    }

  if (max_pair.first != -1) {
    if (table_swap)
      swap_table(max_pair.first, max_pair.second);
    else
      swap(max_pair.first, max_pair.second);
    return true;
  }

  return false;
}
