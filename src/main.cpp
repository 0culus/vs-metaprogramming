#include <iostream>

#include "merge_sort.hpp"
#include "tuple_utilities.hpp"

#define DEBUG 1

int main() {
  using ints = std::integer_sequence<int, 8,7,33,6,22,5,55,4,3,2,22,1,0>;
  ints i;

  // sort the thing
  std::cout << "\nsorted list: " << compile_sort(i) << '\n';

  // unique / contains
  std::cout << std::boolalpha << '\n' <<
    all_unique_tuple<std::tuple<int, char, std::string>>::value << '\n' <<
    all_unique_tuple<std::tuple<int, char, std::string, signed>>::value << '\n' <<
    tuple_contains<std::string, std::tuple<int, char, std::string, ints>>::value << '\n';

  // for-each
  auto t = std::make_tuple(10, 'c', "Hello, World!\n");
  std::cout.put('\n');
  for_each_tuple_element(t, [](auto&& e){std::cout<<e<<'\n';});

#ifdef DEBUG
  system("pause"); // stop debugging session from exiting immediately
#endif
}
