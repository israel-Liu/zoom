// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_ALLOCATOR_ALLOCATOR_ALLOCATOR_CHECK_H_
#define BRICK_ALLOCATOR_ALLOCATOR_ALLOCATOR_CHECK_H_

#include "brick/base_export.h"

namespace base {
namespace allocator {

BRICK_EXPORT bool IsAllocatorInitialized();

}  // namespace allocator
}  // namespace base

#endif  // BRICK_ALLOCATOR_ALLOCATOR_ALLOCATOR_CHECK_H_
