#include <chrono>
#include <cstdio>
#include <fstream>
#include <future>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "file_load_system/async_file_load.hpp"
#include "file_load_system/file_load_system.hpp"
#include "file_load_system/smart_file.hpp"
#include "locale_handling/locale_handling.hpp"
#include "serialization/json_serializer.hpp"
#include "serialization/serializer.hpp"
#include "test/class_test.hpp"
#include "utils/stopwatch.hpp"
#include "utils/logger.hpp"

int main(int argc, char **argv) {

  LocaleHandling::SetStartupLocale();
  FileLoadSystem::SetupFileLoadSystem("KC", "Da Game");
  Logging::Logger::SetupLogging(FileLoadSystem::GetTempDirectory());

  LOG(Logging::Logger::GetDefaultLogger(), 0, u8"0 ñuuuuum 1\n2 cámara 3");

  std::fstream fs, fs2;
  std::string s;
  std::stringstream ss;

  fs.open("test.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  fs2.open("test2.txt",
           std::fstream::in | std::fstream::out | std::fstream::app);

  ss << fs.rdbuf();
  ss << u8"\n\u0148angara\n\u0148o\u0148o" << '\0';
  std::string utf8String = ss.str();
  fs2 << utf8String;

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

  printf("%s\n", utf8String.c_str());

  std::cout << FileLoadSystem::GetCurrentDirectory().generic_u8string()
            << std::endl;

  std::cout << FileLoadSystem::GetExecutableDirectory().generic_u8string()
            << std::endl;

  std::cout << FileLoadSystem::GetTempDirectory().generic_u8string()
            << std::endl;

  {
    FileLoadSystem::path testPath = FileLoadSystem::GetExecutableDirectory() /
                                    FileLoadSystem::CreatePath("prueba.txt");

    FileLoadSystem::SmartWriteFile f = FileLoadSystem::OpenWriteText(testPath);

    if (f.IsValid()) {

      if (FileLoadSystem::Fwrite(utf8String.c_str(), sizeof(char),
                                 utf8String.length(),
                                 f.Get()) != utf8String.length()) {
        std::cout << "Error Writting to File " << testPath.generic_u8string()
                  << std::endl;
      } else {
        std::cout << "Successful Writting to File "
                  << testPath.generic_u8string() << std::endl;
        std::cout << "It Contains " << utf8String << std::endl;
      }

    } else {
      std::cout << "Invalid File " << testPath.generic_u8string() << std::endl;
    }
  }

  FileLoadSystem::FileLoadSystemHandler handler;

  FileLoadSystem::path testPath = handler.CreatePath("a/b/c");
  handler.CreateDirectories(FileLoadSystem::GetExecutableDirectory() /
                            testPath);

  FileLoadSystem::SmartReadFile testFile;

  std::cout << "Size: " << sizeof(testFile) << " VS " << sizeof(std::FILE *)
            << std::endl;

  {

    FileLoadSystem::FileLoadSystemHandler fsHandler;
    FileLoadSystem::path p = FileLoadSystem::GetExecutableDirectory() /
                             fsHandler.CreatePath("prueba.txt");

    // Thread Example
    std::future<FileLoadSystem::FileAndData> future =
        FileLoadSystem::ReadFileWithAlloc(p);

    std::cout << "waiting...\n";
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "waiting x2...\n";
    std::future_status status;
    do {
      status = future.wait_for(std::chrono::seconds(1));
      if (status == std::future_status::deferred) {
        std::cout << "deferred\n";
      } else if (status == std::future_status::timeout) {
        std::cout << "timeout\n";
      } else if (status == std::future_status::ready) {
        std::cout << "ready!\n";
      }
    } while (status != std::future_status::ready);

    std::cout << "result is " << future.get().m_data.get() << '\n';
  }

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
