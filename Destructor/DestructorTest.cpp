#include <iostream>

// Trivial destructor
struct S {
  int x;
  int y;
};

int *getFieldPtr(S &p [[clang::lifetimebound]]) { return &p.x; }

int *trivial_destructor_dangling() {
  S s{1, 2};
  return getFieldPtr(s);
}

/*
DestructorTest.cpp:13:3: warning: Returning value bound to 's' that will go out
of scope [alpha.cplusplus.UseAfterLifetimeEnd] 13 |   return getFieldPtr(s); |
^~~~~~~~~~~~~~~~~~~~~ DestructorTest.cpp:12:3: note: 's' initialized here 12 |
S s{1, 2}; |   ^~~ DestructorTest.cpp:13:3: note: Returning value bound to 's'
that will go out of scope 13 |   return getFieldPtr(s); | ^~~~~~~~~~~~~~~~~~~~~
*/

// Non-trivial destructor virtual
struct Base {
  virtual ~Base() {}
  int x = 1;
};

struct Derived : Base {
  int y = 2;
  ~Derived() override {}
};

int *getBasePtr(Base &b [[clang::lifetimebound]]) { return &b.x; }

int *virtual_destructor_dangling() {
  Derived d;
  return getBasePtr(d);
}

/*
DestructorTest.cpp:39:3: warning: Returning value bound to 'd' that will go out
of scope [alpha.cplusplus.UseAfterLifetimeEnd] 39 |   return getBasePtr(d); |
^~~~~~~~~~~~~~~~~~~~ DestructorTest.cpp:39:3: note: Returning value bound to 'd'
that will go out of scope 39 |   return getBasePtr(d); |   ^~~~~~~~~~~~~~~~~~~~
*/

// Explicit destructor call
struct T {
  int x = 1;
  ~T() {}
};

void placement_new() {
  alignas(T) unsigned char buffer[sizeof(T)];
  T *p = new (buffer) T{1};
  p->~T();
  T *p2 = new (buffer) T{2};
  int x = p2->x;
}

int placenement_new_err() {
  alignas(T) unsigned char buffer[sizeof(T)];
  T *p = new (buffer) T{1};
  p->~T();
  return p->x;
}
