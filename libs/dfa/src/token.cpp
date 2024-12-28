#include "lexer/dfa/token.hpp"

namespace lexer::dfa
{
Token::Token(const std::size_t id) : id_{id}
{}

std::size_t Token::id() const
{
    return id_;
}

bool Token::operator==(const Token& other) const
{
    return id_ == other.id_;
}

} // namespace lexer::dfa
