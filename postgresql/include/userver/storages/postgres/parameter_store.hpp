#pragma once

/// @file userver/storages/postgres/parameter_store.hpp
/// @brief @copybrief storages::postgres::ParameterStore

#include <userver/storages/postgres/detail/query_parameters.hpp>
#include <userver/storages/postgres/io/type_traits.hpp>
#include <userver/storages/postgres/io/user_types.hpp>
#include <userver/utils/strong_typedef.hpp>

USERVER_NAMESPACE_BEGIN

namespace storages::postgres {

/// Class for dynamic parameter list construction
class ParameterStore {
 public:
  /// @brief Adds a parameter to the end of the parameter list.
  /// @note Currently only built-in/system types are supported.
  template <typename T>
  ParameterStore& PushBack(const T& param) {
    static_assert(
        io::traits::kIsMappedToSystemType<T>,
        "Currently only built-in types can be used in ParameterStore");
    data_.Write(kNoUserTypes, param);
    return *this;
  }

  /// Returns whether the parameter list is empty.
  bool IsEmpty() const { return data_.Size() == 0; }

  /// Returns current size of the list.
  size_t Size() const { return data_.Size(); }

  /// @cond
  const detail::DynamicQueryParameters& GetInternalData() const {
    return data_;
  }
  /// @endcond

 private:
  static UserTypes kNoUserTypes;

  detail::DynamicQueryParameters data_;
};

}  // namespace storages::postgres

USERVER_NAMESPACE_END
