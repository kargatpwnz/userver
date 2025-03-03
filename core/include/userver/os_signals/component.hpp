#pragma once

/// @file userver/os_signals/component.hpp
/// @brief @copybrief os_signals::ProcessorComponent

#include <csignal>

#include <userver/components/component_fwd.hpp>
#include <userver/components/impl/component_base.hpp>
#include <userver/os_signals/processor.hpp>

USERVER_NAMESPACE_BEGIN

namespace os_signals {

/// @ingroup userver_components
///
/// @brief A storage for Processor listeners signals
///
/// Declaration in static config file may be skipped.
///
/// @see @ref md_en_userver_os_signals
class ProcessorComponent final : public components::impl::ComponentBase {
 public:
  static constexpr auto kName = "os-signal-processor";

  ProcessorComponent(const components::ComponentConfig& config,
                     const components::ComponentContext& context);

  os_signals::Processor& Get();

 private:
  os_signals::Processor manager_;
};

}  // namespace os_signals

template <>
inline constexpr auto
    components::kConfigFileMode<os_signals::ProcessorComponent> =
        ConfigFileMode::kNotRequired;

USERVER_NAMESPACE_END
