#include "llvm/ADT/STLExtras.h"
#include <vector>

std::vector<int> getTempVector() { return {1, 2, 3}; }

// map_range over a temporary container.
void map_range_over_temporary() {
  for (auto x : llvm::map_range(getTempVector(), [](int n) { return n * 2; })) {
    (void)x;
  }
}

// Nested adaptors.
void nested_map_filter_range() {
  std::vector<int> v{1, 2, 3, 4};
  for (auto x : llvm::map_range(
           llvm::make_filter_range(v, [](int n) { return n % 2 == 0; }),
           [](int n) { return n * 2; })) {
    (void)x;
  }
}

// Lambda capturing a local by reference.
void lambda_capture_by_reference_dangling() {
  std::vector<int> v{1, 2, 3};
  int *danglingTarget = nullptr;
  {
    int local = 10;
    danglingTarget = &local;
  }
  for (auto x : llvm::map_range(
           v, [danglingTarget](int n) { return n + *danglingTarget; })) {
    (void)x;
  }
}

// Lambda capturing by value Captured is copied into the lambda so it is safe.
void lambda_capture_by_value_safe() {
  std::vector<int> v{1, 2, 3};
  int captured = 10;
  for (auto x :
       llvm::map_range(v, [captured](int n) { return n + captured; })) {
    (void)x;
  }
}

// map_range over a struct's member.
struct Container {
  std::vector<int> data() { return {1, 2, 3}; }
};

void map_range_over_struct_member() {
  Container c;
  for (auto x : llvm::map_range(c.data(), [](int n) { return n * 2; })) {
    (void)x;
  }
}
