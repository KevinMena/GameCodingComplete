#pragma once
#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP 1

#include <chrono>
#include <utility>
#include <vector>

/* Namespace for Stopwatch related things */
namespace Stopwatch {

/* Nanoseconds */
using ns = typename std::chrono::nanoseconds;

/* Microseconds */
using us = typename std::chrono::microseconds;

/* Milliseconds */
using ms = typename std::chrono::milliseconds;

/* Seconds */
using sec = typename std::chrono::seconds;

/* Stopwatch to count time based on std::chrono */
class Stopwatch {
public:
  /* Type of Clock we are using */
  using clock = typename std::chrono::high_resolution_clock;

  /* Type of each time point we take from our clock */
  using time_pt = typename clock::time_point;

  /* The basic precision of our clock */
  using precision_pt = ns;

  /* The representation in basic integer type of a duration of our clock */
  using elapsed_pt = typename precision_pt::rep;

  /* Where we save each time point taken from our clock */
  using laps_pt = typename std::vector<time_pt>;

  /* Creates a Stopwatch that reserves an estimatedLapsCount of space for lap
   * counting. Starts if it is wanted to */
  explicit inline Stopwatch(laps_pt::size_type estimatedLapsCount = 1,
                            bool start = true) {
    m_laps.reserve(estimatedLapsCount);
    m_startTime = clock::time_point::min();
    if (start) {
      Start();
    }
  }

  /* Creates a Stopwatch that uses container for lap counting. Starts if it is
   * wanted to */
  inline Stopwatch(laps_pt &&container, bool start = true) noexcept {
    m_laps = std::move_if_noexcept(container);
    m_startTime = clock::time_point::min();
    if (start) {
      Start();
    }
  }

  /* Creates a Stopwatch by moving other Stopwatch */
  inline Stopwatch(Stopwatch &&other) noexcept {
    m_laps = std::move_if_noexcept(other.m_laps);
    m_startTime = std::move_if_noexcept(other.m_startTime);
  }

  /* Assigns a Stopwatch by moving other Stopwatch */
  inline Stopwatch &operator=(Stopwatch &&other) {
    if (this != &other) {
      m_laps = std::move_if_noexcept(other.m_laps);
      m_startTime = std::move_if_noexcept(other.m_startTime);
    }
    return *this;
  }

  /* Doesn't make sense to Copy a Stopwatch */
  Stopwatch(const Stopwatch &other) = delete;

  /* Doesn't make sense to Copy Assign a Stopwatch */
  Stopwatch &operator=(const Stopwatch &other) = delete;

  ~Stopwatch() = default;

  /* If the Clock is Started */
  inline bool IsStarted() {
    return (m_startTime.time_since_epoch() != clock::duration(0));
  }

  /*
    Start our clock and return the elapsed time of the last clock if its valid
  */
  template <typename precision_t = precision_pt,
            typename elapsed_t = typename precision_t::rep>
  inline elapsed_t Start() {
    time_pt now = clock::now();
    precision_t elapsed =
        std ::chrono::duration_cast<precision_t>(now - m_startTime);
    m_laps.clear();
    m_startTime = clock::now();
    m_laps.push_back(m_startTime);
    return elapsed.count();
  }

  /* Elapsed Time From the start of the Clock, keep running */
  template <typename precision_t = precision_pt,
            typename elapsed_t = typename precision_t::rep>
  inline elapsed_t Elapsed() const {
    return std ::chrono::duration_cast<precision_t>(clock::now() - m_startTime)
        .count();
  }

  /*
    Stops our clock and return the elapsed time
  */
  template <typename precision_t = precision_pt,
            typename elapsed_t = typename precision_t::rep>
  inline elapsed_t Stop() {
    time_pt now = clock::now();
    precision_t elapsed =
        std ::chrono::duration_cast<precision_t>(now - m_startTime);
    m_startTime = clock::time_point::min();
    return elapsed.count();
  }

  /* Count a Lap, keep running */
  inline void Lap() { m_laps.push_back(clock::now()); }

  /*
    Get all the collected Laps
  */
  template <typename precision_t = precision_pt,
            typename elapsed_t = typename precision_t::rep>
  inline std::vector<elapsed_t> GetLaps() {

    const laps_pt::size_type size = m_laps.size();

    // No laps taken
    if (size <= 1) {
      return {};
    }

    std::vector<elapsed_t> elapsedLaps;

    elapsedLaps.reserve(size - 1);

    for (laps_pt::size_type i = 1; i < size; ++i) {
      elapsedLaps.push_back(
          std::chrono::duration_cast<precision_t>(m_laps[i] - m_laps[i - 1])
              .count());
    }

    return elapsedLaps;
  }

  /*
    Get all the collected Laps in a container if it cans
  */
  template <typename precision_t = precision_pt,
            typename elapsed_t = typename precision_t::rep>
  inline bool GetLaps(std::vector<elapsed_t> &elapsedLaps) {

    const laps_pt::size_type size = m_laps.size();

    if (elapsedLaps.capacity() < size - 1) {
      return false;
    }

    elapsedLaps.clear();

    // No laps taken
    if (size <= 1) {
      return true;
    }

    for (laps_pt::size_type i = 1; i < size; ++i) {
      elapsedLaps[i - 1] =
          std::chrono::duration_cast<precision_t>(m_laps[i] - m_laps[i - 1])
              .count();
    }

    return true;
  }

private:
  /* Time at the start of Stopwatch */
  time_pt m_startTime;

  /* Time at the end of Stopwatch */
  laps_pt m_laps;
};

} // namespace Stopwatch

#endif // !STOPWATCH_HPP