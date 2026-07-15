struct Container {
  int *Stored = nullptr;
  void store(int *p [[clang::lifetime_capture_by(this)]]) { Stored = p; }
};

void heap_dangling_source_capture_by() {
  Container *c = new Container();
  int *heap_i = new int(5);
  c->store(heap_i);
  delete heap_i;
  (void)c->Stored; // This should warn
  delete c;
}

/*
Currently no warning.
*/
