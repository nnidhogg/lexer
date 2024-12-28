#include "lexer/nfa/label.hpp"

#include <stdexcept>

namespace lexer::nfa
{
bool Epsilon::operator==(const Epsilon&) const noexcept
{
    return true;
}

Label::Label(const Epsilon e) noexcept : variant_{e}
{}

Label::Label(const Symbol_t s) noexcept : variant_{s}
{}

Label Label::epsilon() noexcept
{
    return Label{Epsilon{}};
}

bool Label::is_symbol() const noexcept
{
    return std::holds_alternative<Symbol_t>(variant_);
}

bool Label::is_epsilon() const noexcept
{
    return std::holds_alternative<Epsilon>(variant_);
}

Label::Symbol_t Label::symbol() const
{
    return std::get<Symbol_t>(variant_);
}

const Label::Variant_t& Label::variant() const noexcept
{
    return variant_;
}

bool Label::operator==(const Label& other) const noexcept
{
    return variant_ == other.variant_;
}

} // namespace lexer::nfa
