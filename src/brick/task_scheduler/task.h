// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_TASK_SCHEDULER_TASK_H_
#define BRICK_TASK_SCHEDULER_TASK_H_

#include "brick/base_export.h"
#include "brick/callback.h"
#include "brick/location.h"
#include "brick/macros.h"
#include "brick/memory/ref_counted.h"
#include "brick/pending_task.h"
#include "brick/sequenced_task_runner.h"
#include "brick/single_thread_task_runner.h"
#include "brick/task_scheduler/task_traits.h"
#include "brick/time/time.h"

namespace base {
namespace internal {

// A task is a unit of work inside the task scheduler. Support for tracing and
// profiling inherited from PendingTask.
struct BRICK_EXPORT Task : public PendingTask {
  // |posted_from| is the site the task was posted from. |task| is the closure
  // to run. |traits_in| is metadata about the task. |delay| is a delay that
  // must expire before the Task runs. If |delay| is non-zero and the shutdown
  // behavior in |traits| is BLOCK_SHUTDOWN, the shutdown behavior is
  // automatically adjusted to SKIP_ON_SHUTDOWN.
  Task(const Location& posted_from,
       OnceClosure task,
       const TaskTraits& traits,
       TimeDelta delay);

  // Task is move-only to avoid mistakes that cause reference counts to be
  // accidentally bumped.
  Task(Task&& other) noexcept;

  ~Task();

  Task& operator=(Task&& other);

  // The TaskTraits of this task.
  TaskTraits traits;

  // The delay that must expire before the task runs.
  TimeDelta delay;

  // The time at which the task was inserted in its sequence. For an undelayed
  // task, this happens at post time. For a delayed task, this happens some
  // time after the task's delay has expired. If the task hasn't been inserted
  // in a sequence yet, this defaults to a null TimeTicks.
  TimeTicks sequenced_time;

  // A reference to the SequencedTaskRunner or SingleThreadTaskRunner that
  // posted this task, if any. Used to set ThreadTaskRunnerHandle and/or
  // SequencedTaskRunnerHandle while the task is running.
  // Note: this creates an ownership cycle
  //   Sequence -> Task -> TaskRunner -> Sequence -> ...
  // but that's okay as it's broken when the Task is popped from its Sequence
  // after being executed which means this cycle forces the TaskRunner to stick
  // around until all its tasks have been executed which is a requirement to
  // support TaskRunnerHandles.
  scoped_refptr<SequencedTaskRunner> sequenced_task_runner_ref;
  scoped_refptr<SingleThreadTaskRunner> single_thread_task_runner_ref;

 private:
  DISALLOW_COPY_AND_ASSIGN(Task);
};

}  // namespace internal
}  // namespace base

#endif  // BRICK_TASK_SCHEDULER_TASK_H_
