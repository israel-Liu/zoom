// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brick/test/metrics/user_action_tester.h"

#include "brick/bind.h"
#include "brick/bind_helpers.h"
#include "brick/test/test_simple_task_runner.h"

namespace base {

UserActionTester::UserActionTester()
    : task_runner_(new base::TestSimpleTaskRunner),
      action_callback_(
          base::Bind(&UserActionTester::OnUserAction, base::Unretained(this))) {
  base::SetRecordActionTaskRunner(task_runner_);
  base::AddActionCallback(action_callback_);
}

UserActionTester::~UserActionTester() {
  base::RemoveActionCallback(action_callback_);
}

int UserActionTester::GetActionCount(const std::string& user_action) const {
  UserActionCountMap::const_iterator iter = count_map_.find(user_action);
  return iter == count_map_.end() ? 0 : iter->second;
}

void UserActionTester::ResetCounts() {
  count_map_.clear();
}

void UserActionTester::OnUserAction(const std::string& user_action) {
  ++(count_map_[user_action]);
}

}  // namespace base
