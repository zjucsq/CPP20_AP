#pragma once

#include <cstddef>
#include <type_traits>

template <typename... Ts>
struct TypeList {
  struct IsTypeList {};
  using type = TypeList;
  constexpr static size_t size = sizeof...(Ts);
  template <typename... T>
  using append = TypeList<Ts..., T...>;
  template <typename... T>
  using prepend = TypeList<T..., Ts...>;
  template <template <typename...> typename T>
  using to = T<Ts...>;
};

template <typename TypeList>
concept TL = requires {
  typename TypeList::IsTypeList;
  typename TypeList::type;
};

// some help functions
template <TL In, template <typename> class F>
struct Map;

template <template <typename> class F, typename... Ts>
struct Map<TypeList<Ts...>, F> : TypeList<typename F<Ts>::type...> {};

template <TL In, template <typename> class F>
using Map_t = typename Map<In, F>::type;

template <TL In, template <typename> class P, TL Out = TypeList<>>
struct Filter : Out {};

template <template <typename> class P, TL Out, typename H, typename... Ts>
struct Filter<TypeList<H, Ts...>, P, Out>
        : std::conditional_t<P<H>::value, Filter<TypeList<Ts...>, P, typename Out::template append<H>>, Filter<TypeList<Ts...>, P, Out>> {};

template <TL IN, template <typename> typename P, typename S = TypeList<>, typename R = TypeList<>>
struct Partition {
  struct type {
    using satisfied = S;
    using rest = R;
  };
};

template <TL IN, template <typename> class P>
using Partition_t = typename Partition<IN, P>::type;

template <typename H, typename... Ts, template <typename> typename P, typename S, typename R>
struct Partition<TypeList<H, Ts...>, P, S, R> : std::conditional_t<
                                                        P<H>::value,
                                                        Partition<TypeList<Ts...>, P, typename S::template append<H>, R>,
                                                        Partition<TypeList<Ts...>, P, S, typename R::template append<H>>> {};

template <TL Hd, TL Tl>
struct Concat;

template <typename... T, typename... U>
struct Concat<TypeList<T...>, TypeList<U...>> {
  using type = TypeList<T..., U...>;
};

template <TL Hd, TL Tl>
using Concat_t = typename Concat<Hd, Tl>::type;