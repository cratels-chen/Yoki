void f1() {
L1:
  int a = 0;

  if (a == 0) {
    goto L1;
  }
}

void f2() {
L2:
  int a = 0;

  if (a == 0) {
    goto L2;
  }
}

namespace A {
namespace B {
int f3(int a, int b) {
  int result = 0;
  if (a % 2 == 0) {
    result = a + b;
  } else {
    result = a + b + 1;
  }
  return result;
}
}; // namespace B
}; // namespace A
int main(int argc, const char **argv) {
  f1();
  f2();
  auto v = A::B::f3(1, 3);
  return 0;
}
