#include "lexer/nfa/label.hpp"

#include <stdexcept>

namespace lexer::nfa
{
bool Epsilon::operator==(const Epsilon&) const
{
    return true;
}

Label::Label(const Epsilon e) : variant_{e}
{}

Label::Label(const Symbol_t s) : variant_{s}
{}

Label Label::epsilon()
{
    return Label{Epsilon{}};
}

bool Label::is_symbol() const
{
    return std::holds_alternative<Symbol_t>(variant_);
}

bool Label::is_epsilon() const
{
    return std::holds_alternative<Epsilon>(variant_);
}

Label::Symbol_t Label::symbol() const
{
    if (!is_symbol())
    {
        throw std::logic_error("Transition is not a symbol");
    }

    return std::get<Symbol_t>(variant_);
}

const Label::Variant_t& Label::variant() const
{
    return variant_;
}

bool Label::operator==(const Label& other) const
{
    return variant_ == other.variant_;
}

} // namespace lexer::nfa
