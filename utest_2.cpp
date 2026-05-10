#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "vector.h"
#include <sstream>
#include <string>

TEST_CASE("VectorTest, CopyConstructor") {
  const Vector<double> v({0,1,2,3,4,5,6,7,8,9});
  Vector<double> other{v};

  CHECK( other.size() == 10 );
  CHECK( other.empty() == false );

  for (size_t i=0;i<other.size();i++) {
    CHECK(v[i] == other[i]);
  }

  other.pop_back();

  CHECK( v.size() == other.size() + 1 );
}

TEST_CASE("VectorTest, Assignment") {
  const Vector<double> a({0,1,2,3,4,5,6,7,8,9});
  Vector<double> b;
  b = a;

  for(size_t i=0; i < a.size(); ++i)
    CHECK(a[i] == b[i]);

  CHECK(a.empty() == false);
  CHECK(a.size() == 10);
  CHECK(b.size() == 10);

  b.pop_back();

  CHECK(a.size() == 10);
  CHECK(b.size() == 9);
}

TEST_CASE("IteratorTest, DefaultConstructible") {
  Vector<double>::iterator a;
  Vector<double>::const_iterator b;
}

TEST_CASE("IteratorTest, CopyConstructible") {
  Vector<double> v({1,2});
  Vector<double>::iterator a{v.begin()};
  Vector<double>::iterator b{a};
  CHECK_EQ(a, b);
}

TEST_CASE("IteratorTest, CopyAssignable") {
  Vector<double> v({1,2});
  Vector<double>::iterator a{v.begin()};
  Vector<double>::iterator b{v.end()};
  CHECK_NE(a, b);
  a = b;
  CHECK_EQ(a, b);
}

TEST_CASE("IteratorTest, PreIncrement") {
  Vector<double> v({1});
  Vector<double>::iterator a{v.begin()};
  Vector<double>::iterator b{v.end()};
  CHECK_NE(a, b);
  CHECK_EQ(++a, b);
}

TEST_CASE("IteratorTest, PostIncrement") {
  Vector<double> v({1});
  Vector<double>::iterator a{v.begin()};
  Vector<double>::iterator b{v.end()};
  CHECK_NE(a, b);
  CHECK_NE(a++, b);
  CHECK_EQ(a, b);
}

TEST_CASE("IteratorTest, Dereference") {
  Vector<double> v({0,1,2,3,4});
  {
    size_t cnt = 0;
    for(auto it = v.begin(); it != v.end(); ++it) {
      CHECK_EQ(*it, cnt++);
    }
    CHECK_EQ(cnt, 5);
  }
  auto it = v.begin();
  CHECK_EQ(*it, 0);
  *it = 999;
  CHECK_EQ(*it, 999);
}

TEST_CASE("IteratorTest, ArrowOperator") {
  Vector<double> v({0,1,2,3,4});
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-variable"
  Vector<double>::iterator a{v.begin()};
  Vector<double>::iterator b{v.end()};
  #pragma clang diagnostic pop
  {
    size_t cnt = 0;
    for(auto it = v.begin(); it != v.end(); ++it) {
      CHECK_EQ(*(it.operator->()), cnt++);
    }
    CHECK_EQ(cnt, 5);
  }
}

/**
 * ConstIterator
 */
TEST_CASE("ConstIteratorTest, CopyConstructible") {
  Vector<double> v({1,2});
  Vector<double>::const_iterator a{v.begin()};
  Vector<double>::const_iterator b{a};
  CHECK_EQ(a, b);
}

TEST_CASE("ConstIteratorTest, CopyAssignable") {
  Vector<double> v({1,2});
  Vector<double>::const_iterator a{v.begin()};
  Vector<double>::const_iterator b{v.end()};
  CHECK_NE(a, b);
  a = b;
  CHECK_EQ(a, b);
}

TEST_CASE("ConstIteratorTest, PreIncrement") {
  Vector<double> v({1});
  Vector<double>::const_iterator a{v.begin()};
  Vector<double>::const_iterator b{v.end()};
  CHECK_NE(a, b);
  CHECK_EQ(++a, b);
}

TEST_CASE("ConstIteratorTest, PostIncrement") {
  Vector<double> v({1});
  Vector<double>::const_iterator a{v.begin()};
  Vector<double>::const_iterator b{v.end()};
  CHECK_NE(a, b);
  CHECK_NE(a++, b);
  CHECK_EQ(a, b);
}

TEST_CASE("ConstIteratorTest, Dereference") {
  Vector<double> v({0,1,2,3,4});
  Vector<double>::const_iterator beg{v.begin()};
  Vector<double>::const_iterator end{v.end()};
  {
    size_t cnt = 0;
    for(; beg != end; ++beg) {
      CHECK_EQ(*beg, cnt++);
    }
    CHECK_EQ(cnt, 5);
  }
}

TEST_CASE("ConstIteratorTest, ArrowOperator") {
  Vector<double> v({0,1,2,3,4});
  Vector<double>::const_iterator beg{v.begin()};
  Vector<double>::const_iterator end{v.end()};
  {
    size_t cnt = 0;
    for(; beg != end; ++beg) {
      CHECK_EQ(*(beg.operator->()), cnt++);
    }
    CHECK_EQ(cnt, 5);
  }
}

TEST_CASE("IteratorTest, TypeConversion") {
  Vector<double> v({1});
  Vector<double>::iterator it{v.begin()};
  Vector<double>::const_iterator cit{it};

  //call Vector::const_iterator's operators
  CHECK(!(cit != it));
  CHECK(  cit == it);

  //call Vector::iterator's operators
  CHECK(!(it != cit));
  CHECK(  it == cit);
}

TEST_CASE("IteratorTest, Insert/Erase") {
  Vector<double> v{1,2,3};
  auto x = v.insert(++v.begin(),4);
  CHECK_EQ(v[1],4);
  CHECK_EQ(*x,4);
  auto it =v.begin();
  ++it;
  ++it;
  auto y = v.erase(it);
  CHECK_EQ(*y,3);
  CHECK_EQ(v[2],3);
}

TEST_CASE("IteratorTest, OutputOperator") {
	Vector<double> v{3.14, 0, 0.5, 1, 1.5, 2.71};
	std::stringstream str_out;
	str_out << v;
	std::stringstream str;
	str << "[";
	bool first{true};
	for (const auto& val : v) {
		if (first)
			first = false;
		else
			str << ", ";
		str << val;
	}
	str << "]";
	CHECK_EQ(str_out.str(), str.str());
}
