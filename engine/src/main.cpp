#include <cstdio>
#include <vector>

#include "test/class_test.hpp"
#include "utils/stopwatch.hpp"
#include "serialization/serializer.hpp"
#include "serialization/json_serializer.hpp"

int main(int argc, char **argv) {

  printf("I'm here: %s\n", argv[0]);
  printf("I have %d arguments\n", argc);

  int big_count = 1000000;

  class_test test(1);
  std::vector<int> vec;
  //vec.reserve(big_count);
  Stopwatch::Stopwatch watch(argc, true);

  for (int i = 1; i < big_count; ++i) {
    vec.push_back(i);
    watch.Lap();
  }
  Stopwatch::ns::rep total = watch.Stop();

  auto times = watch.GetLaps<Stopwatch::ns>();

  Stopwatch::ns::rep sum = 0;

  for (int i = 0; i < times.size(); i++)
  {
    sum += times[i];
  }
  
  printf("Average Lap %lf ns\n", static_cast<long double>(sum) /  static_cast<long double>(times.size()) );
  printf("Average Total %lf ns\n", static_cast<long double>(total) /  static_cast<long double>(times.size()) );

  test.say_msg("Probando");

  Serializer::JsonSerializer ser;

  ser.SetEntry("Memingo", 7, 0);

  ser.SetInt("hola", 4, 42);

  ser.SetArray("array", 5);

  for (size_t i = 0; i < 4; i++)
  {
    ser.SetBool(nullptr, 0, (i&1) == 0);
  }

  ser.CloseArray();

  ser.Close();

  ser.CompilePretty();

  Serializer::s_size size;

  ser.GetSize(&size);

  char* resp = new char[size+1];

  resp[size] = '\0';

  ser.GetText(resp);

  printf("%s\n", resp);

  delete[] resp;

  return 0;
}
