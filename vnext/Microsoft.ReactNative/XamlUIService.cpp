// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"
#include "XamlUIService.h"
#include "XamlUIService.g.cpp"
#include <Modules/NativeUIManager.h>
#include <Modules/PaperUIManagerModule.h>
#include "DynamicWriter.h"
#include "ShadowNodeBase.h"
#include "Views/ShadowNodeBase.h"

namespace winrt::Microsoft::ReactNative::implementation {

XamlUIService::XamlUIService(Mso::CntPtr<Mso::React::IReactContext> &&context) noexcept : m_context(context) {}

xaml::DependencyObject XamlUIService::ElementFromReactTag(int64_t reactTag) noexcept {
  if (auto uiManager = ::Microsoft::ReactNative::GetNativeUIManager(*m_context).lock()) {
    auto shadowNode = uiManager->getHost()->FindShadowNodeForTag(reactTag);
    if (!shadowNode)
      return nullptr;

    return static_cast<::Microsoft::ReactNative::ShadowNodeBase *>(shadowNode)->GetView();
  }
  return nullptr;
}

/*static*/ winrt::Microsoft::ReactNative::XamlUIService XamlUIService::FromContext(IReactContext context) {
  return context.Properties()
      .Get(XamlUIService::XamlUIServiceProperty().Handle())
      .try_as<winrt::Microsoft::ReactNative::XamlUIService>();
}

void XamlUIService::DispatchEvent(
    xaml::FrameworkElement const &view,
    hstring const &eventName,
    JSValueArgWriter const &eventDataArgWriter) noexcept {
  folly::dynamic eventData; // default to NULLT
  if (eventDataArgWriter != nullptr) {
    auto eventDataWriter = winrt::make_self<DynamicWriter>();
    eventDataArgWriter(*eventDataWriter);
    eventData = eventDataWriter->TakeValue();
  }

  m_context->DispatchEvent(unbox_value<int64_t>(view.Tag()), to_string(eventName), std::move(eventData));
}

/*static*/ ReactPropertyId<XamlUIService> XamlUIService::XamlUIServiceProperty() noexcept {
  static ReactPropertyId<XamlUIService> uiManagerProperty{L"ReactNative.UIManager", L"XamlUIManager"};
  return uiManagerProperty;
}

ReactPropertyId<xaml::XamlRoot> XamlRootProperty() noexcept {
  static ReactPropertyId<xaml::XamlRoot> propId{L"ReactNative.UIManager", L"XamlRoot"};
  return propId;
}

/*static*/ void XamlUIService::SetXamlRoot(
    IReactPropertyBag const &properties,
    xaml::XamlRoot const &xamlRoot) noexcept {
  winrt::Microsoft::ReactNative::ReactPropertyBag(properties).Set(XamlRootProperty(), xamlRoot);
}

/*static*/ xaml::XamlRoot XamlUIService::GetXamlRoot(IReactPropertyBag const &properties) noexcept {
  return winrt::Microsoft::ReactNative::ReactPropertyBag(properties).Get(XamlRootProperty());
}

} // namespace winrt::Microsoft::ReactNative::implementation
