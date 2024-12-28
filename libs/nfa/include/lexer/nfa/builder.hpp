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

    [[nodiscard]] std::size_t init_state() const;

    [[nodiscard]] std::size_t next_state() const;

    [[nodiscard]] const Nfa::Transitions_t& transitions() const;

    [[nodiscard]] const Nfa::Accept_states_t& accept_states() const;

    [[nodiscard]] std::size_t step_state();

    Builder& add_transition(std::size_t from, const Label& label, std::size_t to);

    Builder& add_epsilon_transition(std::size_t from, std::size_t to);

    Builder& add_accept_state(std::size_t accept_state);

    Builder& add_accept_state(std::size_t accept_state, const Token& token);

    Builder& set_accept_states(Nfa::Accept_states_t accept_states);

    Builder& set_accept_token(const Token& token);

    [[nodiscard]] Builder offset(std::size_t offset) const;

    [[nodiscard]] Builder append(const Builder& other) const;

    [[nodiscard]] Builder merge(const Builder& other) const;

    [[nodiscard]] Nfa build() const;

private:
    Builder(std::size_t init_state, std::size_t next_state, Nfa::Transitions_t transitions,
            Nfa::Accept_states_t accept_states);

    std::size_t init_state_;

    std::size_t next_state_;

    Nfa::Transitions_t transitions_;

    Nfa::Accept_states_t accept_states_;
};

} // namespace lexer::nfa

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_BUILDER_HPP
