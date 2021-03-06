// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brick/win/core_winrt_util.h"

#include "brick/win/com_init_util.h"
#include "brick/win/scoped_com_initializer.h"
#include "brick/win/windows_version.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
namespace win {

TEST(CoreWinrtUtilTest, PreloadFunctions) {
  if (GetVersion() < VERSION_WIN8)
    EXPECT_FALSE(ResolveCoreWinRTDelayload());
  else
    EXPECT_TRUE(ResolveCoreWinRTDelayload());
}

TEST(CoreWinrtUtilTest, RoInitializeAndUninitialize) {
  if (GetVersion() < VERSION_WIN8)
    return;

  ASSERT_TRUE(ResolveCoreWinRTDelayload());
  ASSERT_HRESULT_SUCCEEDED(base::win::RoInitialize(RO_INIT_MULTITHREADED));
  AssertComApartmentType(ComApartmentType::MTA);
  base::win::RoUninitialize();
  AssertComApartmentType(ComApartmentType::NONE);
}

}  // namespace win
}  // namespace base
