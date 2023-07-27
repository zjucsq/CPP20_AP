#include <algorithm>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <vector>

// template <std::input_iterator... InputIt, std::invocable<InputIt...>
// Operation, std::output_iterator , class Tp> >

int main() {
  std::vector<int> v{1, 2, 3, 4, 5};
  std::transform(v.begin(), v.end(), v.begin(), [](int x) { return 2 * x; });
  std::for_each(v.begin(), v.end(),
                [](int x) { std::cout << x << ' ' << std::endl; });
}