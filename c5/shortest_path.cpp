#include <type_traits>

template <char ID> struct Node {
  constexpr static char id = ID;
};

using A = Node<'A'>;
using B = Node<'B'>;
using C = Node<'C'>;
using D = Node<'D'>;
using E = Node<'E'>;
