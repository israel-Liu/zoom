// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_TEST_NATIVE_LIBRARY_TEST_UTILS_H_
#define BRICK_TEST_NATIVE_LIBRARY_TEST_UTILS_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#define NATIVE_LIBRARY_TEST_ALWAYS_EXPORT __declspec(dllexport)
#else
#define NATIVE_LIBRARY_TEST_ALWAYS_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

extern NATIVE_LIBRARY_TEST_ALWAYS_EXPORT int g_native_library_exported_value;

// A function which increments an internal counter value and returns its value.
// The first call returns 1, then 2, etc.
NATIVE_LIBRARY_TEST_ALWAYS_EXPORT int NativeLibraryTestIncrement();

}  // extern "C"

#endif  // BRICK_TEST_NATIVE_LIBRARY_TEST_UTILS_H_
