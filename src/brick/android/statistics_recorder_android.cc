// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "brick/android/jni_string.h"
#include "brick/metrics/histogram_base.h"
#include "brick/metrics/statistics_recorder.h"
#include "brick/sys_info.h"
#include "jni/StatisticsRecorderAndroid_jni.h"

using base::android::JavaParamRef;
using base::android::ConvertUTF8ToJavaString;

namespace base {
namespace android {

static ScopedJavaLocalRef<jstring> JNI_StatisticsRecorderAndroid_ToJson(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    jint verbosityLevel) {
  return ConvertUTF8ToJavaString(
      env, base::StatisticsRecorder::ToJSON(
               static_cast<JSONVerbosityLevel>(verbosityLevel)));
}

}  // namespace android
}  // namespace base
