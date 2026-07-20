int *test_func(int *p [[clang::lifetimebound]]);

int *heap_dangling_source_lifetimebound() {
  int *i = new int(5);
  int *p = test_func(i);
  delete i;
  return p;
}

/*
Currently no warning
*/
