//=============================================================================
// FILE:
//      input1.c
//
// DESCRIPTION:
//      Sample input file, taken from: https://github.com/banach-space/llvm-tutor
//
// License: MIT
//=============================================================================

#include <unistd.h>
#include <stdio.h>

int foo(int a) {
  return a * 2;
}

int bar(int a, int b) {
  return (a + foo(b) * 2);
}

int fez(int a, int b, int c) {
  char buf[32] = {0};
  read(0, buf, 0x100);
  return buf[(a+b+c)%3];
}

int main(int argc, char *argv[]) {
  int a = 123;
  int ret = 0;

  ret += foo(a);
  ret += bar(a, ret);
  ret += fez(a, ret, 123);
  printf("ret: %d\n", ret);

  return 0;
}
