#include <cstdio>

#include "test/class_test.hpp"

int main(int argc, char **argv) {

  printf("I'm here: %s\n", argv[0]);
  printf("I have %d arguments\n", argc);

  class_test test(1);

  for (int i = 1; i < argc; ++i) {
    printf("\t%s\n", argv[i]);
  }

  test.say_msg("Probando");

  return 0;
}
