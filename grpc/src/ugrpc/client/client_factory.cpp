#include <userver/ugrpc/client/client_factory.hpp>

#include <userver/engine/async.hpp>
#include <userver/logging/level_serialization.hpp>
#include <userver/yaml_config/yaml_config.hpp>

#include <ugrpc/impl/logging.hpp>
#include <ugrpc/impl/to_string.hpp>

USERVER_NAMESPACE_BEGIN

namespace ugrpc::client {

namespace {

grpc::ChannelArguments MakeChannelArgs(
    const yaml_config::YamlConfig& channel_args) {
  grpc::ChannelArguments args;
  if (!channel_args.IsMissing()) {
    for (const auto& [key, value] : Items(channel_args)) {
      if (value.IsInt64()) {
        args.SetInt(ugrpc::impl::ToGrpcString(key), value.As<int>());
      } else {
        args.SetString(ugrpc::impl::ToGrpcString(key), value.As<std::string>());
      }
    }
  }
  return args;
}

}  // namespace

ClientFactoryConfig Parse(const yaml_config::YamlConfig& value,
                          formats::parse::To<ClientFactoryConfig>) {
  ClientFactoryConfig config;
  config.channel_args = MakeChannelArgs(value["channel-args"]);
  config.native_log_level =
      value["native-log-level"].As<logging::Level>(config.native_log_level);
  return config;
}

ClientFactory::ClientFactory(ClientFactoryConfig&& config,
                             engine::TaskProcessor& channel_task_processor,
                             grpc::CompletionQueue& queue,
                             utils::statistics::Storage& statistics_storage)
    : channel_task_processor_(channel_task_processor),
      queue_(queue),
      channel_cache_(std::move(config.credentials), config.channel_args),
      client_statistics_storage_(statistics_storage) {
  ugrpc::impl::SetupNativeLogging();
  ugrpc::impl::UpdateNativeLogLevel(config.native_log_level);
}

impl::ChannelCache::Token ClientFactory::GetChannel(
    const std::string& endpoint) {
  // Spawn a blocking task creating a gRPC channel
  // This is third party code, no use of span inside it
  return engine::AsyncNoSpan(channel_task_processor_,
                             [&] { return channel_cache_.Get(endpoint); })
      .Get();
}

}  // namespace ugrpc::client

USERVER_NAMESPACE_END
