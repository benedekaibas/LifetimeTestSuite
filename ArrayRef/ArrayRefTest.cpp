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

/*
Currently the checker does not emit report for any of the ArrayRef test cases.
*/
