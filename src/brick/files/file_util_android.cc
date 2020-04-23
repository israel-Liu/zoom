// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brick/files/file_util.h"

#include "brick/files/file_path.h"
#include "brick/path_service.h"

namespace base {

bool GetShmemTempDir(bool executable, base::FilePath* path) {
  return PathService::Get(base::DIR_CACHE, path);
}

}  // namespace base
