#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP

#include "dfa.hpp"
#include "label.hpp"
#include "token.hpp"

namespace lexer::dfa
{
class Builder
{
public:
    Builder();

    [[nodiscard]] Dfa::State_t init_state() const;

    [[nodiscard]] Dfa::State_t next_state();

    Builder& add_transition(Dfa::State_t from, const Label& label, Dfa::State_t to);

    Builder& add_accept_state(Dfa::State_t accept_state, const Token& token);

    [[nodiscard]] Dfa build() const;

private:
    Dfa::State_t init_state_;

    Dfa::State_t next_state_;

    Dfa::Transitions_t transitions_;

    Dfa::Accept_states_t accept_states_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP
