template<class T> using reference_pointer = shared_ptr<T>;
template<class T> reference_pointer<T> new_reference(T value) {
  return make_shared<T>(move(value));
}
struct P {
  int a, b;
  P(int _a, int _b) : a(_a), b(_b) {}
} p(2, 3);

#ifdef REFERENCE_POINTER_EXAMPLE
reference_pointer<int> a = new_reference(5);
reference_pointer<P> b = new_reference(P(1, 2));
#endif
