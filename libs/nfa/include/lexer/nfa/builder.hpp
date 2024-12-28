#ifndef LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_BUILDER_HPP
#define LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_BUILDER_HPP

#include "label.hpp"
#include "nfa.hpp"

namespace lexer::nfa
{
class Builder
{
public:
    Builder();

    [[nodiscard]] Nfa::State_t init_state() const;

    [[nodiscard]] Nfa::State_t next_state();

    [[nodiscard]] const Nfa::Transitions_t& transitions() const;

    [[nodiscard]] const Nfa::Accept_states_t& accept_states() const;

    Builder& add_transition(Nfa::State_t from, const Label& label, Nfa::State_t to);

    Builder& add_epsilon_transition(Nfa::State_t from, Nfa::State_t to);

    Builder& add_accept_state(Nfa::State_t accept_state);

    Builder& add_accept_state(Nfa::State_t accept_state, const Token& token);

    Builder& set_accept_states(Nfa::Accept_states_t accept_states);

    Builder& set_accept_token(const Token& token);

    [[nodiscard]] Builder offset(Nfa::State_t offset) const;

    [[nodiscard]] Builder append(const Builder& other) const;

    [[nodiscard]] Builder merge(const Builder& other) const;

    [[nodiscard]] Nfa build() const;

private:
    Builder(Nfa::State_t init_state, Nfa::State_t next_state, Nfa::Transitions_t transitions,
            Nfa::Accept_states_t accept_states);

    Nfa::State_t init_state_;

    Nfa::State_t next_state_;

    Nfa::Transitions_t transitions_;

    Nfa::Accept_states_t accept_states_;
};

} // namespace lexer::nfa

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_BUILDER_HPP
