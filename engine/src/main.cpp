#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "locale_handling/locale_handling.hpp"
#include "serialization/json_serializer.hpp"
#include "serialization/serializer.hpp"
#include "test/class_test.hpp"
#include "utils/stopwatch.hpp"
#include "file_load_system/file_load_system.hpp"

int main(int argc, char **argv) {

  LocaleHandling::SetStartupLocale();
  FileLoadSystem::SetupFileLoadSystem("KC", "Da Game");

  std::fstream fs, fs2;
  std::string s;
  std::stringstream ss;

  fs.open("test.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  fs2.open("test2.txt",
           std::fstream::in | std::fstream::out | std::fstream::app);

  ss << fs.rdbuf() << u8"\n\u0148angara\n\u0148o\u0148o" << '\0';
  fs2 << ss.str();

  {
    std::string testStr(u8"0 ñuuuuum 1\n2 cámara 3");

    int i, j, count, step;

    char str[1000];

    count = ::sscanf_s(testStr.c_str(), "%d%s%d%n", &i, str, 1000, &j, &step);

    if (count != 3) {
      std::cout << "fail: " << count << std::endl;
    }

    fs2 << "\n" << i << " " << std::string(str) << " " << j << "\n";

    count = ::sscanf_s(testStr.c_str() + step, "%d%s%d", &i, str, 1000, &j);

    if (count != 3) {
      std::cout << "fail: " << count << std::endl;
    }

    fs2 << "\n" << i << " " << std::string(str) << " " << j << "\n";
    fs2 << "\n" << testStr;
  }

  fs.close();
  fs2.close();

  printf("%s\n", ss.str().c_str());

  std::cout << FileLoadSystem::GetCurrentDirectory().generic_u8string() << std::endl;

  std::cout << FileLoadSystem::GetExecutableDirectory().generic_u8string() << std::endl;

  std::cout << FileLoadSystem::GetTempDirectory().generic_u8string() << std::endl;

  FileLoadSystem::FileLoadSystemHandler handler;

  FileLoadSystem::path testPath = handler.CreatePath("a/b/c");
  handler.CreateDirectories(FileLoadSystem::GetExecutableDirectory() / testPath);

  printf("I'm here: %s\n", argv[0]);
  printf("I have %d arguments\n", argc);

  for (int i = 0; i < argc; ++i) {
    printf("\t%s\n", argv[i]);
  }

  int big_count = 1000000;

  class_test test(1);
  std::vector<int> vec;
  // vec.reserve(big_count);
  Stopwatch::Stopwatch watch(argc, true);

  for (int i = 1; i < big_count; ++i) {
    vec.push_back(i);
    watch.Lap();
  }
  Stopwatch::ns::rep total = watch.Stop();

  auto times = watch.GetLaps<Stopwatch::ns>();

  Stopwatch::ns::rep sum = 0;

  for (int i = 0; i < times.size(); i++) {
    sum += times[i];
  }

  printf("Average Lap %lf ns\n", static_cast<long double>(sum) /
                                     static_cast<long double>(times.size()));
  printf("Average Total %lf ns\n", static_cast<long double>(total) /
                                       static_cast<long double>(times.size()));

  test.say_msg("Probando");

  Serializer::JsonSerializer ser;

  ser.SetEntry("Memingo", 7, 0);

  ser.SetInt("hola", 4, 42);

  ser.SetArray("array", 5);

  for (size_t i = 0; i < 4; i++) {
    ser.SetBool(nullptr, 0, (i & 1) == 0);
  }

  ser.CloseArray();

  ser.CloseEntry();

  ser.CompilePretty();

  Serializer::s_size size;

  ser.GetSize(&size);

  char *resp = new char[size + 1];

  resp[size] = '\0';

  ser.GetText(resp);

  printf("%s\n", resp);

  delete[] resp;

  return 0;
}
