#include "lexer/dfa/label.hpp"

namespace lexer::dfa
{
Label::Label(const Symbol_t s) noexcept : symbol_{s}
{}

Label::Symbol_t Label::symbol() const noexcept
{
    return symbol_;
}

bool Label::operator==(const Label& other) const noexcept
{
    return symbol_ == other.symbol_;
}

} // namespace lexer::dfa
