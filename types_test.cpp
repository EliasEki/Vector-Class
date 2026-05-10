#include<iostream>
#include"vector.h"
using namespace std;

int main() {

  [[maybe_unused]] Vector<double>::value_type val;
  [[maybe_unused]] Vector<double>::size_type size;
  [[maybe_unused]] Vector<double>::difference_type diff;
  [[maybe_unused]] Vector<double>::reference ref = val;
  [[maybe_unused]] Vector<double>::const_reference cref = val;
  [[maybe_unused]] Vector<double>::pointer ptr = &val;
  [[maybe_unused]] Vector<double>::const_pointer cptr = &val;
  [[maybe_unused]] Vector<double>::iterator *it;
  [[maybe_unused]] Vector<double>::const_iterator *cit;

  return 0;
}
