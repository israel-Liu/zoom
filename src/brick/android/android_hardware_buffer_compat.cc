// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brick/android/android_hardware_buffer_compat.h"

#include "brick/android/build_info.h"
#include "brick/lazy_instance.h"
#include "brick/logging.h"

#include <dlfcn.h>

namespace base {

namespace {

static base::LazyInstance<AndroidHardwareBufferCompat>::Leaky g_compat =
    LAZY_INSTANCE_INITIALIZER;

}  // namespace

AndroidHardwareBufferCompat::AndroidHardwareBufferCompat() {
  DCHECK(IsSupportAvailable());

  // TODO(klausw): If the Chromium build requires __ANDROID_API__ >= 26 at some
  // point in the future, we could directly use the global functions instead of
  // dynamic loading. However, since this would be incompatible with pre-Oreo
  // devices, this is unlikely to happen in the foreseeable future, so just
  // unconditionally use dynamic loading.

  // cf. brick/android/linker/modern_linker_jni.cc
  void* main_dl_handle = dlopen(nullptr, RTLD_NOW);

  *reinterpret_cast<void**>(&allocate_) =
      dlsym(main_dl_handle, "AHardwareBuffer_allocate");
  DCHECK(allocate_);

  *reinterpret_cast<void**>(&acquire_) =
      dlsym(main_dl_handle, "AHardwareBuffer_acquire");
  DCHECK(acquire_);

  *reinterpret_cast<void**>(&describe_) =
      dlsym(main_dl_handle, "AHardwareBuffer_describe");
  DCHECK(describe_);

  *reinterpret_cast<void**>(&lock_) =
      dlsym(main_dl_handle, "AHardwareBuffer_lock");
  DCHECK(lock_);

  *reinterpret_cast<void**>(&recv_handle_) =
      dlsym(main_dl_handle, "AHardwareBuffer_recvHandleFromUnixSocket");
  DCHECK(recv_handle_);

  *reinterpret_cast<void**>(&release_) =
      dlsym(main_dl_handle, "AHardwareBuffer_release");
  DCHECK(release_);

  *reinterpret_cast<void**>(&send_handle_) =
      dlsym(main_dl_handle, "AHardwareBuffer_sendHandleToUnixSocket");
  DCHECK(send_handle_);

  *reinterpret_cast<void**>(&unlock_) =
      dlsym(main_dl_handle, "AHardwareBuffer_unlock");
  DCHECK(unlock_);
}

// static
bool AndroidHardwareBufferCompat::IsSupportAvailable() {
  return base::android::BuildInfo::GetInstance()->sdk_int() >=
         base::android::SDK_VERSION_OREO;
}

// static
AndroidHardwareBufferCompat AndroidHardwareBufferCompat::GetInstance() {
  return g_compat.Get();
}

void AndroidHardwareBufferCompat::Allocate(const AHardwareBuffer_Desc* desc,
                                           AHardwareBuffer** out_buffer) {
  DCHECK(IsSupportAvailable());
  allocate_(desc, out_buffer);
}

void AndroidHardwareBufferCompat::Acquire(AHardwareBuffer* buffer) {
  DCHECK(IsSupportAvailable());
  acquire_(buffer);
}

void AndroidHardwareBufferCompat::Describe(const AHardwareBuffer* buffer,
                                           AHardwareBuffer_Desc* out_desc) {
  DCHECK(IsSupportAvailable());
  describe_(buffer, out_desc);
}

int AndroidHardwareBufferCompat::Lock(AHardwareBuffer* buffer,
                                      uint64_t usage,
                                      int32_t fence,
                                      const ARect* rect,
                                      void** out_virtual_address) {
  DCHECK(IsSupportAvailable());
  return lock_(buffer, usage, fence, rect, out_virtual_address);
}

int AndroidHardwareBufferCompat::RecvHandleFromUnixSocket(
    int socket_fd,
    AHardwareBuffer** out_buffer) {
  DCHECK(IsSupportAvailable());
  return recv_handle_(socket_fd, out_buffer);
}

void AndroidHardwareBufferCompat::Release(AHardwareBuffer* buffer) {
  DCHECK(IsSupportAvailable());
  release_(buffer);
}

int AndroidHardwareBufferCompat::SendHandleToUnixSocket(
    const AHardwareBuffer* buffer,
    int socket_fd) {
  DCHECK(IsSupportAvailable());
  return send_handle_(buffer, socket_fd);
}

int AndroidHardwareBufferCompat::Unlock(AHardwareBuffer* buffer,
                                        int32_t* fence) {
  DCHECK(IsSupportAvailable());
  return unlock_(buffer, fence);
}

}  // namespace base
