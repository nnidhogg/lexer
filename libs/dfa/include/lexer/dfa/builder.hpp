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

    [[nodiscard]] std::size_t init_state() const;

    [[nodiscard]] std::size_t next_state() const;

    [[nodiscard]] const Dfa::Transitions_t& transitions() const;

    [[nodiscard]] const Dfa::Accept_states_t& accept_states() const;

    [[nodiscard]] std::size_t step_state();

    void add_transition(std::size_t from, const Label& label, std::size_t to);

    void add_accept_state(std::size_t accept_state, const Token& token);

    [[nodiscard]] Dfa build() const;

private:
    Builder(std::size_t init_state, std::size_t next_state, Dfa::Transitions_t transitions,
            Dfa::Accept_states_t accept_states);

    std::size_t init_state_;

    std::size_t next_state_;

    Dfa::Transitions_t transitions_;

    Dfa::Accept_states_t accept_states_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP
