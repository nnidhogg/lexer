#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_TOKEN_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_TOKEN_HPP

#include <cstddef>

namespace lexer::dfa
{
/**
 * @brief Represents a token in the DFA, identified by a unique ID.
 */
class Token
{
public:
    /**
     * @brief Constructs a token with the given ID.
     * @param id The unique identifier for the token.
     */
    explicit Token(std::size_t id) noexcept;

    /**
     * @brief Returns the unique identifier of the token.
     * @return The token's ID.
     */
    [[nodiscard]] std::size_t id() const noexcept;

    /**
     * @brief Equality comparison operator for tokens.
     * @param other The token to compare with.
     * @return True if the IDs are equal, false otherwise.
     */
    bool operator==(const Token& other) const noexcept;

private:
    std::size_t id_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_TOKEN_HPP
