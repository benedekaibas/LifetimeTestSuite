// Whole struct “by value return” (LazyCompoundVal)
struct S {
  int *p;
};

S makeView(int &x [[clang::lifetimebound]]) { return S{&x}; }

S whole_struct_return_lazycompoundval() {
  int local = 5;
  return makeView(local); // Error here
}

/*
The `whole_struct_return_lazyCompoundVal` and the `getFieldPtr` cases are identical. The difference is that
the return type is different: S and an int pointer. 
A pointer return is a &SymRegion{conj_$6{int *, LC1, S2503, #1}}. `SVal.getAsRegion()` succeeds on it. 
The problem is that an aggregate value is nonloc::LazyCompoundVal and neither getAsRegion() nor getAsSymbol()
covers it.
*/

// Field/member subobjects (FieldRiegon)
struct Pair {
  int a;
  int b;
};

int *getFieldPtr(Pair &p [[clang::lifetimebound]]) { return &p.a; }

int *field_subobject_dangling() {
  Pair pair{3, 5};
  return getFieldPtr(
      pair); // warning: Returning value bound to 'pair' that will go out of
             // scope [alpha.cplusplus.UseAfterLifetimeEnd]
}

// Base class subobject (different subregion than FieldRegion)
struct Base {
  int x;
};
struct Derived : Base {
  int y;
};

int *getBasePtr(Derived &d [[clang::lifetimebound]]) {
  return &static_cast<Base &>(d).x;
}

int *base_subobject_dangling() {
  Derived derived{};
  return getBasePtr(
      derived); // warning: Returning value bound to 'derived' that will go out
                // of scope [alpha.cplusplus.UseAfterLifetimeEnd]
}

// Nested subobjects (like Struct inside a Struct holding a pointer)
struct Inner {
  int val;
};
struct Outer {
  Inner inner;
};

int *getNestedFieldPtr(Outer &o [[clang::lifetimebound]]) {
  return &o.inner.val;
}

int *nested_subobject_dangling() {
  Outer outer{};
  return getNestedFieldPtr(
      outer); // warning: Returning value bound to 'outer' that will go out of
              // scope [alpha.cplusplus.UseAfterLifetimeEnd]
}

// Array-member subobjects (struct containing an array ptr points inside the
// array)
struct Buffer {
  int arr[5];
};

int *getArrayElementPtr(Buffer &b [[clang::lifetimebound]]) {
  return &b.arr[0];
}

int *array_member_subobject_dangling() {
  Buffer buf{};
  return getArrayElementPtr(
      buf); // warning: Returning value bound to 'buf' that will go out of scope
            // [alpha.cplusplus.UseAfterLifetimeEnd]
}
