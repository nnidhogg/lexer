#ifndef LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_NFA_HPP
#define LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_NFA_HPP

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "label.hpp"
#include "token.hpp"

namespace lexer::nfa
{
class Nfa
{
    using Key_t = std::pair<std::size_t, Label>;

    struct Hash
    {
        std::size_t operator()(const Key_t& key) const noexcept;
    };

public:
    using States_t = std::unordered_set<std::size_t>;

    using Transitions_t = std::unordered_map<Key_t, States_t, Hash>;

    using Accept_states_t = std::unordered_map<std::size_t, std::optional<Token>>;

    Nfa(std::size_t init_state, Transitions_t transitions, Accept_states_t accept_states);

    [[nodiscard]] std::size_t init_state() const;

    [[nodiscard]] const Transitions_t& transitions() const;

    [[nodiscard]] const Accept_states_t& accept_states() const;

    [[nodiscard]] static States_t epsilon_closure(const Nfa& nfa, const States_t& states);

    [[nodiscard]] static States_t advance(const Nfa& nfa, const States_t& states, char symbol);

    [[nodiscard]] static std::optional<Token> find_token(const Nfa& nfa, const States_t& states);

private:
    std::size_t init_state_;

    Transitions_t transitions_;

    Accept_states_t accept_states_;
};

} // namespace lexer::nfa

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_NFA_HPP
