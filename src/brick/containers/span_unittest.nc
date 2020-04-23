// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is a "No Compile Test" suite.
// http://dev.chromium.org/developers/testing/no-compile-tests

#include "brick/containers/span.h"

#include <array>
#include <set>
#include <vector>

namespace base {

class Base {
};

class Derived : Base {
};

#if defined(NCTEST_DEFAULT_SPAN_WITH_NON_ZERO_STATIC_EXTENT_DISALLOWED)  // [r"fatal error: static_assert failed \"Invalid Extent\""]

// A default constructed span must have an extent of 0 or dynamic_extent.
void WontCompile() {
  span<int, 1> span;
}

#elif defined(NCTEST_SPAN_FROM_ARRAY_WITH_NON_MATCHING_STATIC_EXTENT_DISALLOWED) // [r"fatal error: no matching constructor for initialization of 'span<int, 1>'"]

// A span with static extent constructed from an array must match the size of
// the array.
void WontCompile() {
  int array[] = {1, 2, 3};
  span<int, 1> span(array);
}

#elif defined(NCTEST_SPAN_FROM_STD_ARRAY_WITH_NON_MATCHING_STATIC_EXTENT_DISALLOWED) // [r"fatal error: no matching constructor for initialization of 'span<int, 2>'"]

// A span with static extent constructed from std::array must match the size of
// the array.
void WontCompile() {
  std::array<int, 3> array = {1, 2, 3};
  span<int, 2> span(array);
}

#elif defined(NCTEST_SPAN_FROM_CONST_STD_ARRAY_WITH_NON_MATCHING_STATIC_EXTENT_DISALLOWED) // [r"fatal error: no matching constructor for initialization of 'span<const int, 2>'"]

// A span with static extent constructed from std::array must match the size of
// the array.
void WontCompile() {
  const std::array<int, 3> array = {1, 2, 3};
  span<const int, 2> span(array);
}

#elif defined(NCTEST_SPAN_FROM_OTHER_SPAN_WITH_MISMATCHING_EXTENT_DISALLOWED) // [r"fatal error: no matching constructor for initialization of 'span<int, 4>'"]

// A span with static extent constructed from another span must match the
// extent.
void WontCompile() {
  std::array<int, 3> array = {1, 2, 3};
  span<int, 3> span3(array);
  span<int, 4> span4(span3);
}

#elif defined(NCTEST_DYNAMIC_SPAN_TO_STATIC_SPAN_DISALLOWED)  // [r"fatal error: no matching constructor for initialization of 'span<int, 3>'"]

// Converting a dynamic span to a static span should not be allowed.
void WontCompile() {
  span<int> dynamic_span;
  span<int, 3> static_span(dynamic_span);
}

#elif defined(NCTEST_DERIVED_TO_BRICK_CONVERSION_DISALLOWED)  // [r"fatal error: no matching constructor for initialization of 'span<base::Base \*>'"]

// Internally, this is represented as a pointer to pointers to Derived. An
// implicit conversion to a pointer to pointers to Base must not be allowed.
// If it were allowed, then something like this would be possible.
//   Cat** cats = GetCats();
//   Animals** animals = cats;
//   animals[0] = new Dog();  // Uhoh!
void WontCompile() {
  span<Derived*> derived_span;
  span<Base*> base_span(derived_span);
}

#elif defined(NCTEST_PTR_TO_CONSTPTR_CONVERSION_DISALLOWED)  // [r"fatal error: no matching constructor for initialization of 'span<const int \*>'"]

// Similarly, converting a span<int*> to span<const int*> requires internally
// converting T** to const T**. This is also disallowed, as it would allow code
// to violate the contract of const.
void WontCompile() {
  span<int*> non_const_span;
  span<const int*> const_span(non_const_span);
}

#elif defined(NCTEST_CONST_CONTAINER_TO_MUTABLE_CONVERSION_DISALLOWED)  // [r"fatal error: no matching constructor for initialization of 'span<int>'"]

// A const container should not be convertible to a mutable span.
void WontCompile() {
  const std::vector<int> v = {1, 2, 3};
  span<int> span(v);
}

#elif defined(NCTEST_STD_SET_CONVERSION_DISALLOWED)  // [r"fatal error: no matching constructor for initialization of 'span<int>'"]

// A std::set() should not satisfy the requirements for conversion to a span.
void WontCompile() {
  std::set<int> set;
  span<int> span(set);
}

#elif defined(NCTEST_STATIC_FRONT_WITH_EXCEEDING_COUNT_DISALLOWED)  // [r"fatal error: static_assert failed \"Count must not exceed Extent\""]

// Static first called on a span with static extent must not exceed the size.
void WontCompile() {
  std::array<int, 3> array = {1, 2, 3};
  span<int, 3> span(array);
  auto first = span.first<4>();
}

#elif defined(NCTEST_STATIC_LAST_WITH_EXCEEDING_COUNT_DISALLOWED)  // [r"fatal error: static_assert failed \"Count must not exceed Extent\""]

// Static last called on a span with static extent must not exceed the size.
void WontCompile() {
  std::array<int, 3> array = {1, 2, 3};
  span<int, 3> span(array);
  auto last = span.last<4>();
}

#elif defined(NCTEST_STATIC_SUBSPAN_WITH_EXCEEDING_OFFSET_DISALLOWED)  // [r"fatal error: static_assert failed \"Offset must not exceed Extent\""]

// Static subspan called on a span with static extent must not exceed the size.
void WontCompile() {
  std::array<int, 3> array = {1, 2, 3};
  span<int, 3> span(array);
  auto subspan = span.subspan<4>();
}

#elif defined(NCTEST_STATIC_SUBSPAN_WITH_EXCEEDING_COUNT_DISALLOWED)  // [r"fatal error: static_assert failed \"Count must not exceed Extent - Offset\""]

// Static subspan called on a span with static extent must not exceed the size.
void WontCompile() {
  std::array<int, 3> array = {1, 2, 3};
  span<int, 3> span(array);
  auto subspan = span.subspan<0, 4>();
}

#elif defined(NCTEST_AS_WRITABLE_BYTES_WITH_CONST_CONTAINER_DISALLOWED)  // [r"fatal error: no matching function for call to 'as_writable_bytes'"]

// as_writable_bytes should not be possible for a const container.
void WontCompile() {
  const std::vector<int> v = {1, 2, 3};
  span<uint8_t> bytes = as_writable_bytes(make_span(v));
}

#elif defined(NCTEST_MAKE_SPAN_FROM_SET_CONVERSION_DISALLOWED)  // [r"fatal error: no matching function for call to 'make_span'"]

// A std::set() should not satisfy the requirements for conversion to a span.
void WontCompile() {
  std::set<int> set;
  auto span = make_span(set);
}

#endif

}  // namespace base
