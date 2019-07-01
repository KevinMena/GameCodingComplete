
#include "test/class_test.hpp"

#include <cstdio>

class_test::class_test(int a) {
  _a = a;
  printf("Hello a: %d\n", _a);
}

class_test::~class_test() { printf("Bye a: %d\n", _a); }

void class_test::say_msg(char *msg) { printf("a: %d msg: %s\n", _a, msg); }