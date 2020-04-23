// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdint.h>

#include "brick/android/jni_string.h"
#include "brick/time/time.h"
#include "brick/trace_event/trace_event.h"
#include "jni/EarlyTraceEvent_jni.h"

namespace base {
namespace android {

const char kEarlyJavaCategory[] = "EarlyJava";

static void JNI_EarlyTraceEvent_RecordEarlyEvent(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jstring>& jname,
    jlong begin_time_ns,
    jlong end_time_ns,
    jint thread_id,
    jlong thread_duration_ms) {
  std::string name = ConvertJavaStringToUTF8(env, jname);
  int64_t begin_us = begin_time_ns / 1000;
  int64_t end_us = end_time_ns / 1000;
  int64_t thread_duration_us = thread_duration_ms * 1000;

  INTERNAL_TRACE_EVENT_ADD_WITH_ID_TID_AND_TIMESTAMPS(
      kEarlyJavaCategory, name.c_str(), trace_event_internal::kNoId, thread_id,
      TimeTicks::FromInternalValue(begin_us),
      TimeTicks::FromInternalValue(end_us),
      ThreadTicks::Now() + TimeDelta::FromMicroseconds(thread_duration_us),
      TRACE_EVENT_FLAG_COPY);
}

static void JNI_EarlyTraceEvent_RecordEarlyStartAsyncEvent(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jstring>& jname,
    jlong id,
    jlong timestamp_ns) {
  std::string name = ConvertJavaStringToUTF8(env, jname);
  int64_t timestamp_us = timestamp_ns / 1000;

  TRACE_EVENT_COPY_ASYNC_BEGIN_WITH_TIMESTAMP0(
      kEarlyJavaCategory, name.c_str(), id,
      base::TimeTicks() + base::TimeDelta::FromMicroseconds(timestamp_us));
}

static void JNI_EarlyTraceEvent_RecordEarlyFinishAsyncEvent(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jstring>& jname,
    jlong id,
    jlong timestamp_ns) {
  std::string name = ConvertJavaStringToUTF8(env, jname);
  int64_t timestamp_us = timestamp_ns / 1000;

  TRACE_EVENT_COPY_ASYNC_END_WITH_TIMESTAMP0(
      kEarlyJavaCategory, name.c_str(), id,
      base::TimeTicks() + base::TimeDelta::FromMicroseconds(timestamp_us));
}

}  // namespace android
}  // namespace base
