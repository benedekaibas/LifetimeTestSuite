int *test_func(int *p [[clang::lifetimebound]]);

void heap_dangling_source_lifetimebound() {
  int *heap_i = new int(5);
  int *p = test_func(heap_i);
  delete heap_i;
  (void)p;
}

/*
Currently no warning
*/
