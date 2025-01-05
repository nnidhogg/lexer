#include "lexer/lexer.hpp"

namespace lexer
{
Lexer::Lexer(dfa::Dfa dfa) : dfa_{std::move(dfa)}
{}

const dfa::Dfa& Lexer::dfa() const
{
    return dfa_;
}

} // namespace lexer
