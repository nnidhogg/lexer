#ifndef LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_TOKEN_HPP
#define LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_TOKEN_HPP

#include <cstddef>

namespace lexer::nfa
{
class Token
{
public:
    Token(std::size_t id, std::size_t priority);

    [[nodiscard]] std::size_t id() const;

    [[nodiscard]] std::size_t priority() const;

    bool operator<(const Token& other) const;

    bool operator==(const Token& other) const;

private:
    std::size_t id_;

    std::size_t priority_;
};

} // namespace lexer::nfa

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_TOKEN_HPP
