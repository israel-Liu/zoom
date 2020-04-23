// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brick/native_library.h"

#include "brick/logging.h"

#include "brick/strings/string_util.h"

namespace base {

std::string NativeLibraryLoadError::ToString() const {
  return message;
}

NativeLibrary LoadNativeLibraryWithOptions(const base::FilePath& library_path,
                                           const NativeLibraryOptions& options,
                                           NativeLibraryLoadError* error) {
  NOTIMPLEMENTED();
  if (error)
    error->message = "Not implemented.";
  return nullptr;
}

void UnloadNativeLibrary(NativeLibrary library) {
  NOTIMPLEMENTED();
  DCHECK(!library);
}

void* GetFunctionPointerFromNativeLibrary(NativeLibrary library,
                                          StringPiece name) {
  NOTIMPLEMENTED();
  return nullptr;
}

std::string GetNativeLibraryName(StringPiece name) {
  DCHECK(IsStringASCII(name));
  return name.as_string();
}

std::string GetLoadableModuleName(StringPiece name) {
  return GetNativeLibraryName(name);
}

}  // namespace base
