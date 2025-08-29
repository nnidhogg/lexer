#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP

#include "dfa.hpp"
#include "label.hpp"
#include "token.hpp"

namespace lexer::dfa
{
/**
 * @brief Builder class for constructing DFA objects.
 *
 * Allows incremental construction of a DFA by adding states, transitions, and accept states.
 */
class Builder
{
public:
    /**
     * @brief Constructs a new DFA Builder.
     */
    Builder();

    /**
     * @brief Returns the initial state of the DFA.
     * @return The initial state identifier.
     */
    [[nodiscard]] Dfa::State_t init_state() const;

    /**
     * @brief Generates and returns the next available state identifier.
     * @return The next state identifier.
     */
    [[nodiscard]] Dfa::State_t next_state();

    /**
     * @brief Adds a transition from one state to another on a given label.
     * @param from The source state.
     * @param label The transition label.
     * @param to The destination state.
     * @return Reference to this Builder for chaining.
     */
    Builder& add_transition(Dfa::State_t from, const Label& label, Dfa::State_t to);

    /**
     * @brief Marks a state as an accept state with the associated token.
     * @param accept_state The state to mark as accepting.
     * @param token The token associated with this accept state.
     * @return Reference to this Builder for chaining.
     */
    Builder& add_accept_state(Dfa::State_t accept_state, const Token& token);

    /**
     * @brief Builds and returns the constructed DFA.
     * @return The constructed DFA object.
     */
    [[nodiscard]] Dfa build() const;

private:
    Dfa::State_t init_state_;

    Dfa::State_t next_state_;

    Dfa::Transitions_t transitions_;

    Dfa::Accept_states_t accept_states_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_BUILDER_HPP
