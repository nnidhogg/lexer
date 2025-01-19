#ifndef LEXER_INCLUDE_LEXER_BUILDER_HPP
#define LEXER_INCLUDE_LEXER_BUILDER_HPP

#include <memory>

#include "lexer.hpp"
#include "lexer/dfa/dfa.hpp"
#include "lexer/nfa/builder.hpp"
#include "lexer/regex/regex.hpp"

namespace lexer
{
/**
 * @brief Builder class for constructing a Lexer from regex patterns and tokens.
 *
 * Allows incremental registration of tokens with associated regex patterns and priorities, and builds the final Lexer.
 */
class Builder
{
public:
    /**
     * @brief Registers a token with a regex pattern and priority.
     * @tparam T The token type (enum or integral).
     * @param regex The regex pattern for the token.
     * @param token The token value (enum or integer).
     * @param priority The priority for resolving conflicts (lower is higher priority).
     * @throws std::runtime_error if the regex pointer is invalid.
     */
    template <typename T>
    void add_token(const std::shared_ptr<const regex::Regex>& regex, const T token, const std::size_t priority)
    {
        static_assert(std::is_enum_v<T> || std::is_integral_v<T>, "T must be an integral type or an enum");

        if (!regex)
        {
            throw std::runtime_error("Invalid regex pointer");
        }

        add_token(regex, {static_cast<std::size_t>(token), priority});
    }

    /**
     * @brief Builds and returns the constructed Lexer.
     * @return The constructed Lexer object.
     */
    [[nodiscard]] Lexer build() const;

protected:
    /**
     * @brief Returns the constructed NFA from the registered tokens.
     * @return The constructed NFA object.
     */
    [[nodiscard]] nfa::Nfa nfa() const;

    /**
     * @brief Returns the constructed DFA from the registered tokens.
     * @return The constructed DFA object.
     */
    [[nodiscard]] dfa::Dfa dfa() const;

private:
    /**
     * @brief Internal method to register a token with a regex and NFA token.
     * @param regex The regex pattern.
     * @param token The NFA token.
     */
    void add_token(const std::shared_ptr<const regex::Regex>& regex, const nfa::Token& token);

    /**
     * @brief Converts an NFA to a DFA using subset construction.
     * @param nfa The NFA to convert.
     * @return The constructed DFA.
     */
    [[nodiscard]] static dfa::Dfa subset_construction(const nfa::Nfa& nfa);

    /**
     * @brief Internal NFA builder used to accumulate token patterns.
     */
    nfa::Builder nfa_;
};

} // namespace lexer

#endif // LEXER_INCLUDE_LEXER_BUILDER_HPP
