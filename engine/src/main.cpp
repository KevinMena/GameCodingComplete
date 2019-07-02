#include <cstdio>

#include "test/class_test.hpp"
#include "utils/stopwatch.hpp"

#include <chrono>

int main(int argc, char **argv) {

  printf("I'm here: %s\n", argv[0]);
  printf("I have %d arguments\n", argc);

  class_test test(1);
  Stopwatch::Stopwatch watch(argc, true);

  for (int i = 1; i < argc; ++i) {
    printf("\t%s\n", argv[i]);
    watch.Lap();
  }
  watch.Stop();

  auto times = watch.GetLaps<std::chrono::milliseconds>();

  for (int i = 0; i < times.size(); i++)
  {
    printf("Lap %d: %lld ms\n",i, times[i]);
  }
  

  test.say_msg("Probando");

  return 0;
}
