#ifndef LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_LABEL_HPP
#define LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_LABEL_HPP

#include <variant>

namespace lexer::nfa
{
/**
 * @brief Represents an epsilon (empty string) transition label for NFA transitions.
 */
class Epsilon
{
public:
    /**
     * @brief Equality comparison operator for epsilon labels.
     * @return True if both are epsilon labels.
     */
    bool operator==(const Epsilon&) const;
};

/**
 * @brief Represents a transition label for NFA transitions (symbol or epsilon).
 */
class Label
{
public:
    using Symbol_t = char;

    using Variant_t = std::variant<Symbol_t, Epsilon>;

    /**
     * @brief Constructs a label with the given symbol.
     * @param s The symbol for the label.
     */
    explicit Label(Symbol_t s);

    /**
     * @brief Returns a label representing an epsilon transition.
     * @return An epsilon label.
     */
    [[nodiscard]] static Label epsilon();

    /**
     * @brief Checks if the label is a symbol.
     * @return True if the label is a symbol, false if epsilon.
     */
    [[nodiscard]] bool is_symbol() const;

    /**
     * @brief Checks if the label is an epsilon transition.
     * @return True if the label is epsilon, false otherwise.
     */
    [[nodiscard]] bool is_epsilon() const;

    /**
     * @brief Returns the symbol associated with this label.
     * @return The symbol character.
     * @throws std::bad_variant_access if not a symbol.
     */
    [[nodiscard]] Symbol_t symbol() const;

    /**
     * @brief Returns the underlying variant (symbol or epsilon).
     * @return Reference to the variant.
     */
    [[nodiscard]] const Variant_t& variant() const;

    /**
     * @brief Equality comparison operator for labels.
     * @param other The label to compare with.
     * @return True if the variants are equal, false otherwise.
     */
    bool operator==(const Label& other) const;

private:
    explicit Label(Epsilon e);

    Variant_t variant_;
};

} // namespace lexer::nfa

/**
 * @brief Hash specialization for lexer::nfa::Epsilon to allow use in hash-based containers.
 */
template <>
struct std::hash<lexer::nfa::Epsilon>
{
    std::size_t operator()(const lexer::nfa::Epsilon&) const noexcept { return 0; }
};

/**
 * @brief Hash specialization for lexer::nfa::Label to allow use in hash-based containers.
 */
template <>
struct std::hash<lexer::nfa::Label>
{
    std::size_t operator()(const lexer::nfa::Label& label) const noexcept
    {
        return std::visit([]<typename T>(const T& arg) { return std::hash<std::decay_t<T>>{}(arg); }, label.variant());
    }
};

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_LABEL_HPP
