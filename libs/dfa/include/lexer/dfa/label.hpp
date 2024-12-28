#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_LABEL_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_LABEL_HPP

#include <functional>

namespace lexer::dfa
{
class Label
{
public:
    using Symbol_t = char;

    explicit Label(Symbol_t s);

    [[nodiscard]] Symbol_t symbol() const;

    bool operator==(const Label& other) const;

private:
    Symbol_t symbol_;
};

} // namespace lexer::dfa

template <>
struct std::hash<lexer::dfa::Label>
{
    std::size_t operator()(const lexer::dfa::Label& label) const noexcept
    {
        return std::hash<decltype(label.symbol())>{}(label.symbol());
    }
};

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_LABEL_HPP
