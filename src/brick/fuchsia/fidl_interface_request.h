// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRICK_FUCHSIA_FIDL_INTERFACE_REQUEST_H_
#define BRICK_FUCHSIA_FIDL_INTERFACE_REQUEST_H_

#include "brick/base_export.h"
#include "brick/fuchsia/scoped_zx_handle.h"
#include "brick/macros.h"
#include "brick/strings/string_piece.h"

namespace fidl {

template <typename Interface>
class InterfaceRequest;

template <typename Interface>
class InterfacePtr;

template <typename Interface>
class SynchronousInterfacePtr;

}  // namespace fidl

namespace base {
namespace fuchsia {

// A request for a FIDL interface. FidlInterfaceRequest contains interface name
// and channel handle. Interface consumers create FidlInterfaceRequest when they
// need to connect to a service. FidlInterfaceRequest is resolved when the
// channel is passed to the service implementation, e.g. through
// ComponentContext.
class BRICK_EXPORT FidlInterfaceRequest {
 public:
  template <typename Interface>
  explicit FidlInterfaceRequest(fidl::InterfaceRequest<Interface> request)
      : FidlInterfaceRequest(
            Interface::Name_,
            ScopedZxHandle::FromZxChannel(request.TakeChannel())) {}

  // Creates a new request for |Interface| and binds the client end to the
  // |stub|. |stub| can be used immediately after the request is created, even
  // before the request is passed to the service that implements the interface.
  template <typename Interface>
  explicit FidlInterfaceRequest(fidl::InterfacePtr<Interface>* stub)
      : FidlInterfaceRequest(stub->NewRequest()) {}

  template <typename Interface>
  explicit FidlInterfaceRequest(fidl::SynchronousInterfacePtr<Interface>* stub)
      : FidlInterfaceRequest(stub->NewRequest()) {}

  FidlInterfaceRequest(FidlInterfaceRequest&&);
  ~FidlInterfaceRequest();

  bool is_valid() const { return interface_name_ && channel_; }

  const char* interface_name() const { return interface_name_; }

  // Extracts the channel handle to be passed to service implementation. The
  // request becomes invalid after this call, i.e. TakeChannel() can be called
  // only once.
  ScopedZxHandle TakeChannel();

 private:
  FidlInterfaceRequest(const char* interface_name, ScopedZxHandle channel);

  const char* interface_name_;
  ScopedZxHandle channel_;

  DISALLOW_COPY_AND_ASSIGN(FidlInterfaceRequest);
};

}  // namespace fuchsia
}  // namespace base

#endif  // BRICK_FUCHSIA_FIDL_INTERFACE_REQUEST_H_