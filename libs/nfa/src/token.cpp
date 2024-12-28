#include "lexer/nfa/token.hpp"

namespace lexer::nfa
{
Token::Token(const std::size_t id, const std::size_t priority) : id_{id}, priority_{priority}
{}

std::size_t Token::id() const
{
    return id_;
}

std::size_t Token::priority() const
{
    return priority_;
}

bool Token::operator<(const Token& other) const
{
    return priority_ < other.priority_;
}

bool Token::operator==(const Token& other) const
{
    return id_ == other.id_ && priority_ == other.priority_;
}

} // namespace lexer::nfa
