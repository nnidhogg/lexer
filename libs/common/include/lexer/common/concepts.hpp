#ifndef LEXER_LIBS_COMMON_INCLUDE_LEXER_COMMON_CONCEPTS_HPP
#define LEXER_LIBS_COMMON_INCLUDE_LEXER_COMMON_CONCEPTS_HPP

#include <concepts>
#include <ranges>

namespace lexer::common::concepts
{
template <typename T>
concept Iterator = std::input_iterator<T>;

template <typename T>
concept Iterable = std::ranges::range<T>;

} // namespace lexer::common::concepts

#endif // LEXER_LIBS_COMMON_INCLUDE_LEXER_COMMON_CONCEPTS_HPP
