#ifndef RFL_INTERNAL_HASFIELDS_HPP_
#define RFL_INTERNAL_HASFIELDS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "rfl/internal/all_fields.hpp"
#include "rfl/internal/is_field.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/ptr_tuple_t.hpp"

namespace rfl {
namespace internal {

template <class TupleType, int _i = 0>
constexpr bool has_some_fields() {
  if constexpr (_i == std::tuple_size_v<TupleType>) {
    return false;
  } else {
    using T = std::decay_t<std::tuple_element_t<_i, TupleType>>;
    return is_field_v<T> || has_some_fields<TupleType, _i + 1>();
  }
}

template <class T>
constexpr bool has_fields() {
  if constexpr (is_named_tuple_v<T>) {
    return true;
  } else {
    using TupleType = ptr_tuple_t<T>;
    if constexpr (has_some_fields<TupleType>()) {
      static_assert(
          all_fields<TupleType>(),
          "If some of your fields are annotated using rfl::Field<...>, then "
          "you "
          "must "
          "annotate all of your fields. Also, you cannot combine annotated and "
          "unannotated fields using rfl::Flatten<...>.");
      return true;
    } else {
      return false;
    }
  }
}

}  // namespace internal
}  // namespace rfl

#endif
