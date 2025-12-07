#include <print>
#include <random>

#include "6x8.h"
#include "seating_chart.h"

constexpr auto f_lim = 1500;

int main() {
  auto rd = std::random_device{};
  auto rng = std::mt19937{rd()};

  auto initial_chart = [] {
    std::array<student_t, s_num> chart{};
    for (short i = 0; i < s_num; ++i)
      chart[i] = i;
    return chart;
  }();
  auto seating = seating_chart(initial_chart);

  auto high_score = seating.score();

  auto failure = 0;
  while (true) {
    seating.partial_shuffle(rng);

    while (seating.hill_climb())
      ;

    auto score = seating.score();
    if (score > high_score) {
      high_score = score;
      seating.export_c();
      std::println("New high score: {:.6f}", high_score);
      failure = 0;
    } else
      ++failure;

    if (failure > f_lim) {
      std::println("Restarting...");
      seating.shuffle(rng);
      high_score = seating.score();
    }
  }
}
