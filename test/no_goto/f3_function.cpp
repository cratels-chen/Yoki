#include "f3_function.h"

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
} // namespace B
} // namespace A
