// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"

#include <Modules/NativeUIManager.h>
#include <UI.Xaml.Media.h>
#include <Utils/Helpers.h>
#include <winrt/Windows.Foundation.Metadata.h>

#include <appmodel.h>
#include <processthreadsapi.h>

namespace winrt {
using namespace xaml::Controls::Primitives;
using namespace xaml::Media;
using namespace Windows::Foundation::Metadata;
} // namespace winrt

namespace react::uwp {

// Not only react-native, native modules could set tag too for controls.
// For example, to identify an clicked item, customer may add tag in
// NavigationView since content for the two NavigationViewItem are empty.
//
// <NavigationView>
//  <NavigationViewItem Icon="Accept" Tag="1" />
//  <NavigationViewItem Icon="Accept" Tag="2" />
// </NavigationView>
// Instead of deduce view id directly from FrameworkElement.Tag, this do
// additional check by uimanager.
ReactId getViewId(const Mso::React::IReactContext &context, xaml::FrameworkElement const &fe) {
  ReactId reactId;
  if (auto uiManager = static_cast<NativeUIManager *>(context.NativeUIManager())) {
    if (auto peer = uiManager->reactPeerOrContainerFrom(fe)) {
      reactId.isValid = true;
      reactId.tag = GetTag(peer);
    }
  }
  return reactId;
};

std::int32_t CountOpenPopups() {
  // TODO: Use VisualTreeHelper::GetOpenPopupsFromXamlRoot when running against
  // RS6
  winrt::Windows::Foundation::Collections::IVectorView<xaml::Controls::Primitives::Popup> popups =
      xaml::Media::VisualTreeHelper::GetOpenPopups(xaml::Window::Current());
  return (int32_t)popups.Size();
}

template <uint16_t APIVersion>
bool IsAPIContractVxAvailable() {
  static bool isAPIContractVxAvailableInitialized = false;
  static bool isAPIContractVxAvailable = false;
  if (!isAPIContractVxAvailableInitialized) {
    isAPIContractVxAvailableInitialized = true;
    isAPIContractVxAvailable =
        winrt::ApiInformation::IsApiContractPresent(L"Windows.Foundation.UniversalApiContract", APIVersion);
  }

  return isAPIContractVxAvailable;
}

bool IsAPIContractV5Available() {
  return IsAPIContractVxAvailable<5>();
}

bool IsAPIContractV6Available() {
  return IsAPIContractVxAvailable<6>();
}

bool IsAPIContractV7Available() {
  return IsAPIContractVxAvailable<7>();
}

bool IsAPIContractV8Available() {
  return IsAPIContractVxAvailable<8>();
}

bool IsRS3OrHigher() {
  return IsAPIContractV5Available();
}

bool IsRS4OrHigher() {
  return IsAPIContractV6Available();
}

bool IsRS5OrHigher() {
  return IsAPIContractV7Available();
}

bool Is19H1OrHigher() {
  return IsAPIContractV8Available();
}

bool IsXamlIsland() {
  AppPolicyWindowingModel e;
  if (FAILED(AppPolicyGetWindowingModel(GetCurrentThreadEffectiveToken(), &e)) ||
      e == AppPolicyWindowingModel_ClassicDesktop) {
    return true;
  }
  return false;
}

} // namespace react::uwp
