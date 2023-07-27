#include <__concepts/convertible_to.h>
#include <__concepts/same_as.h>
#include <type_traits>
#include <vector>

// basic
// https://www.jianshu.com/p/826c6c80d089
// https://zhuanlan.zhihu.com/p/266086040
// how to define concept by type_traits
// 一个永远都能匹配成功的concept
template <typename T>
concept always_satisfied = true;
// 一个约束T只能是整数类型的concept，整数类型包括 char, unsigned char, short,
// ushort, int, unsinged int, long等。
template <typename T>
concept integral = std::is_integral_v<T>;
// 一个约束T只能是整数类型，并且是有符号的concept
template <typename T>
concept signed_integral = integral<T> && std::is_signed_v<T>;
// how to use concept
// 约束函数模板方法1
template <integral T> void f(T v);
// 约束函数模板方法2
template <typename T>
  requires integral<T>
void f(T v);
// 约束函数模板方法3
template <typename T>
void f(T v)
  requires integral<T>;
// 直接约束C++14的auto的函数参数
void f(integral auto v);
// 约束模板的auto参数
template <integral auto v> void g();
// 约束auto变量
inline integral auto foo = 5;

// C++20 AP
// how to define concept by type_traits (advanced)
// For C2, both T and U must have 'type' member and one of them are integral,
// then C2 is true. For C1, only one of T and U have 'type' member and are
// integral is OK.
template <typename T, typename U>
concept C1 = std::is_integral_v<typename T::type> ||
             std::is_integral_v<typename U::type>;
template <typename T, typename U>
concept C2 = bool(std::is_integral_v<typename T::type> ||
                  std::is_integral_v<typename U::type>);
// Note that C4 means T has 'type' member and its type is not integral.
template <typename T>
concept C3 = std::is_integral_v<typename T::type>;
template <typename T>
concept C4 = !
std::is_integral_v<typename T::type>;
template <typename T>
concept C5 = !
C3<T>;

// how to define concept by requires
// 1. Simple Requirements
template <typename M>
concept Machine = requires(M m) {
                    m.powerUp();
                    m.powerDown(); // member function
                  };
template <typename T>
concept Animal = requires(T animal) {
                   play(animal); // free function
                   T::count;     // static member
                   animal.age;   // member variable
                 };
template <typename T>
concept Number = requires(T a, T b, T c) {
                   a == a;   // operator==
                   a + b *c; // operator+,*
                 };
// 2. Type Requirements
template <typename T>
concept C_type = requires {
                   typename T::type;        // has a type "type"
                   typename std::vector<T>; // can instantiate vector
                 };
// 3. Compound Requirements
// 这里 ->和type-constraint是可选的.
// {expression} noexcept(optional) -> type-constraint;
template <typename T>
concept C_compound = requires(T x) {
                       { f(x) } -> std::same_as<T>;
                       { g(x) } -> std::convertible_to<T>;
                     };
// 4. Nested Requirements
// type traits; concept; requires; constexpr
template <typename T>
concept C_nested = requires {
                     requires sizeof(T) > sizeof(void *);
                     // sizeof(T) > sizeof(void *);
                     // The upper is wrong, it only evaluate "sizeof(T) >
                     // sizeof(void *)" is legal or not, so it is alway true.
                     requires std::is_trivial_v<T>;
                   };