// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_SYNCHRONIZATION_CANCELLATION_FLAG_H_
#define BRICK_SYNCHRONIZATION_CANCELLATION_FLAG_H_

#include "brick/synchronization/atomic_flag.h"

namespace base {

// Use inheritance instead of "using" to allow forward declaration of "class
// CancellationFlag".
// TODO(fdoray): Replace CancellationFlag with AtomicFlag throughout the
// codebase and delete this file. crbug.com/630251
class CancellationFlag : public AtomicFlag {};

}  // namespace base

#endif  // BRICK_SYNCHRONIZATION_CANCELLATION_FLAG_H_
