#pragma once

#include <type_traits>

#include "type_list.hpp"

template <bool... Bools>
struct and_bools :
  std::is_same<
    type_list<
      typename std::conditional<Bools, std::true_type, std::false_type>::type...
    >,
    type_list<
      typename std::conditional<Bools, std::true_type, std::true_type>::type...
    >
  >
{ };
