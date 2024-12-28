#ifndef LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_LABEL_HPP
#define LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_LABEL_HPP

#include <variant>

namespace lexer::nfa
{
class Epsilon
{
public:
    bool operator==(const Epsilon&) const;
};

class Label
{
public:
    using Symbol_t = char;

    using Variant_t = std::variant<Symbol_t, Epsilon>;

    explicit Label(Symbol_t s);

    [[nodiscard]] static Label epsilon();

    [[nodiscard]] bool is_symbol() const;

    [[nodiscard]] bool is_epsilon() const;

    [[nodiscard]] Symbol_t symbol() const;

    [[nodiscard]] const Variant_t& variant() const;

    bool operator==(const Label& other) const;

private:
    explicit Label(Epsilon e);

    Variant_t variant_;
};

} // namespace lexer::nfa

template <>
struct std::hash<lexer::nfa::Epsilon>
{
    std::size_t operator()(const lexer::nfa::Epsilon&) const noexcept { return 0; }
};

template <>
struct std::hash<lexer::nfa::Label>
{
    std::size_t operator()(const lexer::nfa::Label& label) const noexcept
    {
        return std::visit([]<typename T>(const T& arg) { return std::hash<std::decay_t<T>>{}(arg); }, label.variant());
    }
};

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_LABEL_HPP
