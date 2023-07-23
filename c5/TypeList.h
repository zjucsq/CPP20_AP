#pragma once

#include <cstddef>
#include <type_traits>

template <typename... Ts> struct TypeList {
  struct IsTypeList {};
  using type = TypeList;
  constexpr static size_t size = sizeof...(Ts);
  template <typename... T> using append = TypeList<Ts..., T...>;
  template <typename... T> using prepend = TypeList<T..., Ts...>;
  template <template <typename...> typename T> using to = T<Ts...>;
};

template <typename TypeList>
concept TL = requires {
               typename TypeList::IsTypeList;
               typename TypeList::type;
             };

// some help functions
template <TL In, template <typename> class F> struct Map;

template <template <typename> class F, typename... Ts>
struct Map<TypeList<Ts...>, F> : TypeList<typename F<Ts>::type...> {};

template <TL In, template <typename> class F>
using Map_t = typename Map<In, F>::type;

template <TL In, template <typename> class P, TL Out = TypeList<>>
struct Filter : Out {};

template <template <typename> class P, TL Out, typename H, typename... Ts>
struct Filter<TypeList<H, Ts...>, P, Out>
    : std::conditional_t<
          P<H>::value,
          Filter<TypeList<Ts...>, P, typename Out::template append<H>>,
          Filter<TypeList<Ts...>, P, Out>> {};
