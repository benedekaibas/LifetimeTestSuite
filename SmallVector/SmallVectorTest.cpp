#include "llvm/ADT/SmallVector.h"
using llvm::SmallVector;

// Variable goes out of scope
const int *getSmallVectorData(SmallVector<int, 4> &v [[clang::lifetimebound]]) {
  return v.data();
}

const int *small_vector_stack_dangling() {
  SmallVector<int, 4> local{1, 2, 3};
  return getSmallVectorData(local);
}

/*
SmallVectorTest.cpp:11:3: warning: Returning value bound to 'local' that will go
out of scope [alpha.cplusplus.UseAfterLifetimeEnd] 11 |   return
getSmallVectorData( |   ^~~~~~~~~~~~~~~~~~~~~~~~~~ 12 |       local); //
warning: Returning value bound to 'local' that will go out of |       ~~~~~~
SmallVectorTest.cpp:10:3: note: 'local' initialized here
   10 |   SmallVector<int, 4> local{1, 2, 3};
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~
SmallVectorTest.cpp:11:3: note: Returning value bound to 'local' that will go
out of scope 11 |   return getSmallVectorData( |   ^~~~~~~~~~~~~~~~~~~~~~~~~~ 12
|       local); // warning: Returning value bound to 'local' that will go out of
      |       ~~~~~~
*/

// Reallocation invalidation
int small_vector_reallocation_invalidation() {
  SmallVector<int, 2> v{1, 2};
  const int *p = v.data();
  v.push_back(3);
  return *p;
}
