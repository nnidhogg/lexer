#ifndef LEXER_INCLUDE_LEXER_LEXER_HPP
#define LEXER_INCLUDE_LEXER_LEXER_HPP

#include <optional>

#include "lexer/common/concepts.hpp"
#include "lexer/dfa/dfa.hpp"
#include "lexer/dfa/simulator.hpp"

namespace lexer
{
/**
 * @brief The main Lexer class for tokenizing input using a DFA.
 *
 * Provides methods to tokenize input from iterators or containers, returning the matched token and length.
 */
class Lexer
{
public:
    /**
     * @brief Constructs a Lexer from a DFA.
     * @param dfa The DFA to use for tokenization.
     */
    explicit Lexer(dfa::Dfa dfa) : dfa_{std::move(dfa)} {}

    /**
     * @brief The result type: a pair of the matched token (if any) and the length of the match.
     * @tparam T The token type (enum or integral).
     */
    template <typename T>
    using Result_t = std::pair<std::optional<T>, std::size_t>;

    /**
     * @brief Tokenizes input from a pair of iterators.
     * @tparam T The token type (enum or integral).
     * @tparam Iterator The input iterator type.
     * @param begin Iterator to the beginning of the input.
     * @param end Iterator to the end of the input.
     * @return A pair containing the matched token (if any) and the length of the match.
     */
    template <typename T, common::concepts::Iterator Iterator>
    [[nodiscard]] Result_t<T> tokenize(Iterator begin, Iterator end) const
    {
        static_assert(std::is_enum_v<T> || std::is_integral_v<T>, "T must be an integral type or an enum");

        const auto [token, offset]{dfa::Simulator::run(dfa_, begin, end)};

        return {token ? std::optional<T>{static_cast<T>(token->id())} : std::nullopt, offset};
    }

    /**
     * @brief Tokenizes input from a container.
     * @tparam T The token type (enum or integral).
     * @tparam Container The input container type (must be iterable).
     * @param container The input container.
     * @return A pair containing the matched token (if any) and the length of the match.
     */
    template <typename T, common::concepts::Iterable Container>
    [[nodiscard]] auto tokenize(const Container& container) const
    {
        return tokenize<T>(std::begin(container), std::end(container));
    }

private:
    /**
     * @brief The DFA used for tokenization.
     */
    dfa::Dfa dfa_;
};

} // namespace lexer

#endif // LEXER_INCLUDE_LEXER_LEXER_HPP
