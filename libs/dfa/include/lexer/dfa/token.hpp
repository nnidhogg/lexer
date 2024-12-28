#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_TOKEN_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_TOKEN_HPP

#include <cstddef>

namespace lexer::dfa
{
class Token
{
public:
    explicit Token(std::size_t id);

    [[nodiscard]] std::size_t id() const;

    bool operator==(const Token& other) const;

private:
    std::size_t id_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_TOKEN_HPP
