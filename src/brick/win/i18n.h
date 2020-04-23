// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_WIN_I18N_H_
#define BRICK_WIN_I18N_H_

#include <string>
#include <vector>

#include "brick/base_export.h"

namespace base {
namespace win {
namespace i18n {

// Adds to |languages| the list of user preferred UI languages from MUI, if
// available, falling-back on the user default UI language otherwise.  Returns
// true if at least one language is added.
BRICK_EXPORT bool GetUserPreferredUILanguageList(
    std::vector<std::wstring>* languages);

// Adds to |languages| the list of thread, process, user, and system preferred
// UI languages from MUI, if available, falling-back on the user default UI
// language otherwise.  Returns true if at least one language is added.
BRICK_EXPORT bool GetThreadPreferredUILanguageList(
    std::vector<std::wstring>* languages);

}  // namespace i18n
}  // namespace win
}  // namespace base

#endif  // BRICK_WIN_I18N_H_
