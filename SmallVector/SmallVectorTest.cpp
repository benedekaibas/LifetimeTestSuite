#include "llvm/ADT/SmallVector.h"
using llvm::SmallVector;

// Variable goes out of scope
const int *getSmallVectorData(SmallVector<int, 4> &v [[clang::lifetimebound]]) {
  return v.data();
}

const int *small_vector_stack_dangling() {
  SmallVector<int, 4> local{1, 2, 3};
  return getSmallVectorData(
      local); // warning: Returning value bound to 'local' that will go out of
              // scope [alpha.cplusplus.UseAfterLifetimeEnd]
}

// Reallocation invalidation
int small_vector_reallocation_invalidation() {
  SmallVector<int, 2> v{1, 2};
  const int *p = v.data();
  v.push_back(3);
  return *p;
}
