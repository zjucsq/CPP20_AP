#include "TypeList.h"

int main() {
  using LongList = TypeList<char, float, double, int, char>;
  static_assert(
      std::is_same_v<Map_t<LongList, std::add_pointer>,
                     TypeList<char *, float *, double *, int *, char *>>);
  static_assert(
      std::is_same_v<LongList::append<int, int>,
                     TypeList<char, float, double, int, char, int, int>>);
  static_assert(
      std::is_same_v<
          LongList::append<int, int>::prepend<long, double>,
          TypeList<long, double, char, float, double, int, char, int, int>>);
}
