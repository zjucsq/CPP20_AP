#include <functional>
#include <iostream>
#include <list>
#include <tuple>
#include <type_traits>

#include "TypeList.h"

template <typename T, typename CMP = std::less<>>
void qsort(std::list<T> &l, CMP cmp = CMP()) {
  if (l.size() < 2)
    return;
  auto pivot = l.begin();
  auto it = pivot;
  ++it;
  auto left = std::list<int>{};
  auto right = std::list<int>{};
  while (it != l.end()) {
    if (cmp(*it, *pivot)) {
      auto tmp = *it;
      it = l.erase(it);
      left.push_back(tmp);
    } else {
      ++it;
    }
  }
  right.splice(right.begin(), l, ++l.begin(), l.end());
  qsort(left, cmp);
  qsort(right, cmp);
  l.splice(l.begin(), left);
  l.splice(l.end(), right);
}

template <typename IN, template <typename, typename> typename CMP>
struct QuickSort {
  using type = TypeList<>;
};

template <typename IN, template <typename, typename> typename CMP>
using QuickSort_t = typename QuickSort<IN, CMP>::type;

template <typename H, typename... Ts, template <typename, typename> typename CMP>
class QuickSort<TypeList<H, Ts...>, CMP> {
  template <typename E>
  struct LT {
    static constexpr bool value = CMP<E, H>::value;
  };
  using P = Partition_t<TypeList<Ts...>, LT>;
  // smallerSorted = quicksort [a | a <- xs, a <= x]
  using SmallerSorted = QuickSort_t<typename P::satisfied, CMP>;
  // biggerSorted  = quicksort [a | a <- xs, a > x]
  using BiggerSorted = QuickSort_t<typename P::rest, CMP>;

public:
  // smallerSorted ++ [x] ++ biggerSorted
  using type = Concat_t<typename SmallerSorted::template append<H>, BiggerSorted>;
};

template <typename T, typename U>
class Less {
public:
  static constexpr bool value = sizeof(T) < sizeof(U);
};

template <typename T = int, T... Values>
struct List {
  using type = List;
  constexpr static size_t size = sizeof...(Values);
  template <T... V>
  using append = List<T, Values..., V...>;
  template <T... V>
  using prepend = List<T, V..., Values...>;
};

template <typename IN, template <typename> typename CMP>
struct QSort {
  using type = List<>;
};

template <typename IN, template <typename> typename CMP>
using QSort_t = typename QSort<IN, CMP>::type;

template <typename L, typename CMP, typename R = List<>, typename S = List<>>
struct Partition_List {
  using satisfied = R;
  using rest = S;
};

template <typename T, T hd, T... values, typename CMP, typename R, typename S>
struct Partition_List<List<T, hd, values...>, CMP, R, S> : std::conditional_t<
                                                                   CMP()(hd),
                                                                   Partition_List<List<T, values...>, CMP, typename R::template append<hd>, S>,
                                                                   Partition_List<List<T, values...>, CMP, R, typename S::template append<hd>>> {};

template <typename Hd, typename Tl>
struct Concat_List;

template <typename Type, Type... T, Type... U>
struct Concat_List<List<Type, T...>, List<Type, U...>> {
  using type = List<Type, T..., U...>;
};

template <typename Hd, typename Tl>
using Concat_List_t = typename Concat_List<Hd, Tl>::type;

template <typename T, T pivot, T... values, template <typename> typename CMP>
struct QSort<List<T, pivot, values...>, CMP> {
  struct CMP_curry {
    constexpr bool operator()(T t) {
      return CMP<T>()(pivot, t);
    }
  };
  // constexpr static bool cmp_fn(T t) {
  //   return CMP<T>(pivot, t);
  // };
  bool s = CMP_curry(5);
  using P = Partition_List<List<T, values...>, CMP_curry>;
  using SmallerSorted = QSort_t<typename P::satisfied, CMP>;
  using BiggerSorted = QSort_t<typename P::rest, CMP>;
  using type = Concat_List_t<typename SmallerSorted::template append<pivot>, BiggerSorted>;
};

int main() {
  std::list<int> lst = {9, 5, 2, 7, 1, 3, 8, 4, 6};
  qsort(lst);
  for (int val : lst) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
  static_assert(std::is_same_v<
                TypeList<char, int, double, std::tuple<int, double>>,
                QuickSort_t<TypeList<double, std::tuple<int, double>, char, int>, Less>>);
  static_assert(std::is_same_v<
                List<int, 14, 24, 24, 26, 36, 42, 43, 45, 46, 51, 53, 64, 82>,
                QSort_t<List<int, 42, 64, 36, 43, 53, 46, 26, 82, 45, 24, 14, 51, 24>, std::greater>>);
  return 0;
}