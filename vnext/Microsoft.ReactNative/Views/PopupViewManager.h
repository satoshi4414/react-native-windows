// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include <Views/FrameworkElementViewManager.h>

namespace react::uwp {

class PopupViewManager : public FrameworkElementViewManager {
  using Super = FrameworkElementViewManager;

 public:
  PopupViewManager(const Mso::React::IReactContext &context);

  const char *GetName() const override;
  folly::dynamic GetNativeProps() const override;

  facebook::react::ShadowNode *createShadow() const override;

  folly::dynamic GetExportedCustomDirectEventTypeConstants() const override;
  void SetLayoutProps(
      ShadowNodeBase &nodeToUpdate,
      const XamlView &viewToUpdate,
      float left,
      float top,
      float width,
      float height) override;

 protected:
  XamlView CreateViewCore(int64_t tag) override;
  friend class PopupShadowNode;
};

} // namespace react::uwp
