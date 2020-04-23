// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_SAMPLING_HEAP_PROFILER_SAMPLING_HEAP_PROFILER_H_
#define BRICK_SAMPLING_HEAP_PROFILER_SAMPLING_HEAP_PROFILER_H_

#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

#include "brick/base_export.h"
#include "brick/macros.h"
#include "brick/synchronization/lock.h"
#include "brick/threading/thread_local.h"

namespace base {

template <typename T>
class NoDestructor;

// The class implements sampling profiling of native memory heap.
// It hooks on base::allocator and base::PartitionAlloc.
// When started it selects and records allocation samples based on
// the sampling_interval parameter.
// The recorded samples can then be retrieved using GetSamples method.
class BRICK_EXPORT SamplingHeapProfiler {
 public:
  class BRICK_EXPORT Sample {
   public:
    Sample(const Sample&);
    ~Sample();

    size_t size;   // Allocation size.
    size_t total;  // Total size attributed to the sample.
    std::vector<void*> stack;

   private:
    friend class SamplingHeapProfiler;

    Sample(size_t, size_t total, uint32_t ordinal);

    uint32_t ordinal;
  };

  class SamplesObserver {
   public:
    virtual ~SamplesObserver() = default;
    virtual void SampleAdded(uint32_t id, size_t size, size_t total) = 0;
    virtual void SampleRemoved(uint32_t id) = 0;
  };

  // Must be called early during the process initialization. It creates and
  // reserves a TLS slot.
  static void InitTLSSlot();

  // This is an entry point for plugging in an external allocator.
  // Profiler will invoke the provided callback upon initialization.
  // The callback should install hooks onto the corresponding memory allocator
  // and make them invoke SamplingHeapProfiler::RecordAlloc and
  // SamplingHeapProfiler::RecordFree upon corresponding allocation events.
  //
  // If the method is called after profiler is initialized, the callback
  // is invoked right away.
  static void SetHooksInstallCallback(void (*hooks_install_callback)());

  void AddSamplesObserver(SamplesObserver*);
  void RemoveSamplesObserver(SamplesObserver*);

  uint32_t Start();
  void Stop();
  void SetSamplingInterval(size_t sampling_interval);
  void SuppressRandomnessForTest(bool suppress);

  std::vector<Sample> GetSamples(uint32_t profile_id);

  static void RecordAlloc(void* address, size_t, uint32_t skip_frames = 0);
  static void RecordFree(void* address);

  static SamplingHeapProfiler* GetInstance();

 private:
  using SamplesMap = std::unordered_map<void*, Sample>;

  SamplingHeapProfiler();
  ~SamplingHeapProfiler() = delete;

  static void InstallAllocatorHooksOnce();
  static bool InstallAllocatorHooks();
  static size_t GetNextSampleInterval(size_t base_interval);

  void DoRecordAlloc(size_t total_allocated,
                     size_t allocation_size,
                     void* address,
                     uint32_t skip_frames);
  void DoRecordFree(void* address);
  void RecordStackTrace(Sample*, uint32_t skip_frames);
  SamplesMap& EnsureNoRehashingMap();
  static SamplesMap& samples();

  base::ThreadLocalBoolean entered_;
  base::Lock mutex_;
  std::stack<std::unique_ptr<SamplesMap>> sample_maps_;
  std::vector<SamplesObserver*> observers_;
  uint32_t last_sample_ordinal_ = 0;

  static SamplingHeapProfiler* instance_;

  friend class base::NoDestructor<SamplingHeapProfiler>;

  DISALLOW_COPY_AND_ASSIGN(SamplingHeapProfiler);
};

}  // namespace base

#endif  // BRICK_SAMPLING_HEAP_PROFILER_SAMPLING_HEAP_PROFILER_H_
