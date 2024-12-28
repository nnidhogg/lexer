#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_LABEL_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_LABEL_HPP

#include <functional>

namespace lexer::dfa
{
/**
 * @brief Represents a transition label (symbol) for DFA transitions.
 */
class Label
{
public:
    using Symbol_t = char;

    /**
     * @brief Constructs a label with the given symbol.
     * @param s The symbol for the label.
     */
    explicit Label(Symbol_t s);

    /**
     * @brief Returns the symbol associated with this label.
     * @return The symbol character.
     */
    [[nodiscard]] Symbol_t symbol() const;

    /**
     * @brief Equality comparison operator for labels.
     * @param other The label to compare with.
     * @return True if the symbols are equal, false otherwise.
     */
    bool operator==(const Label& other) const;

private:
    Symbol_t symbol_;
};

} // namespace lexer::dfa

/**
 * @brief Hash specialization for lexer::dfa::Label to allow use in hash-based containers.
 */
template <>
struct std::hash<lexer::dfa::Label>
{
    std::size_t operator()(const lexer::dfa::Label& label) const noexcept
    {
        return std::hash<decltype(label.symbol())>{}(label.symbol());
    }
};

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_LABEL_HPP
