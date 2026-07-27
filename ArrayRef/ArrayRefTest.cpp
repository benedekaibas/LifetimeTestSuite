#include "llvm/ADT/ArrayRef.h"
#include <vector>
using llvm::ArrayRef;

// Directly dangling return
ArrayRef<int> dangling_ptr_length_constr() {
  int local[3] = {1, 2, 3};
  return ArrayRef<int>(local, 3);
}

// Generic container constr
ArrayRef<int> dangling_generic_container_constr() {
  std::vector<int> local{1, 2, 3};
  return ArrayRef<int>(local);
}

// By value function parameter annotated with [[clang::lifetimebound]]
const int *getFirstElement(ArrayRef<int> arr [[clang::lifetimebound]]) {
  return arr.data();
}

const int *array_ref_param_dangling() {
  int local[3] = {1, 2, 3};
  return getFirstElement(local);
}

void s() {
  int *ptr = nullptr;
  {
    int i = 5;
    ptr = &i;
  }
  *ptr = 6;
}

/*
The root cause while the checker stays silent is that getAsRegion() in
checkPostCall return null for by-value aggregate arguments.
*/
