#include "llvm/ADT/StringRef.h"
using llvm::StringRef;
#include <string>

// Dangling temporary
std::string getTempString() { return "Ez egy teszt"; }

StringRef string_ref_dangling_from_temporary() { return getTempString(); }

// Local stack storage dangles
const char *getStringRefData(StringRef s [[clang::lifetimebound]]) {
  return s.data();
}

const char *string_ref_dangling_from_local_stack() {
  char local[] = "Ez egy teszt";
  return getStringRefData(local);
}

// String literal (no warning test case)
const char *string_ref_no_warning() { return getStringRefData("literal"); }

/*
No warning were emitted from the checkers for any of the test cases.
*/
