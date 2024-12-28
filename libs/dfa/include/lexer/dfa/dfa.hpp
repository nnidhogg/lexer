#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_DFA_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_DFA_HPP

#include <optional>
#include <unordered_map>

#include "label.hpp"
#include "token.hpp"

namespace lexer::dfa
{
class Dfa
{
    using Key_t = std::pair<std::size_t, Label>;

    struct Hash
    {
        std::size_t operator()(const Key_t& key) const noexcept;
    };

public:
    using State_t = std::size_t;

    using Transitions_t = std::unordered_map<Key_t, State_t, Hash>;

    using Accept_states_t = std::unordered_map<State_t, Token>;

    Dfa(State_t init_state, Transitions_t transitions, Accept_states_t accept_states);

    [[nodiscard]] State_t init_state() const;

    [[nodiscard]] const Transitions_t& transitions() const;

    [[nodiscard]] const Accept_states_t& accept_states() const;

    [[nodiscard]] static std::optional<State_t> advance(const Dfa& dfa, State_t state, char symbol);

    [[nodiscard]] static std::optional<Token> has_accept_token(const Dfa& dfa, State_t state);

private:
    State_t init_state_;

    Transitions_t transitions_;

    Accept_states_t accept_states_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_DFA_HPP
