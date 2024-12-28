#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_DFA_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_DFA_HPP

#include <optional>
#include <unordered_map>

#include "label.hpp"
#include "token.hpp"

namespace lexer::dfa
{
/**
 * @brief Represents a deterministic finite automaton (DFA).
 *
 * Provides methods for querying states, transitions, and accept states, as well as advancing the DFA.
 */
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

    /**
     * @brief Constructs a DFA with the given initial state, transitions, and accept states.
     * @param init_state The initial state of the DFA.
     * @param transitions The transition table.
     * @param accept_states The accept states and their associated tokens.
     */
    Dfa(State_t init_state, Transitions_t transitions, Accept_states_t accept_states);

    /**
     * @brief Returns the initial state of the DFA.
     * @return The initial state identifier.
     */
    [[nodiscard]] State_t init_state() const;

    /**
     * @brief Returns the transition table of the DFA.
     * @return Reference to the transitions map.
     */
    [[nodiscard]] const Transitions_t& transitions() const;

    /**
     * @brief Returns the accept states and their associated tokens.
     * @return Reference to the accept states map.
     */
    [[nodiscard]] const Accept_states_t& accept_states() const;

    /**
     * @brief Advances the DFA from a given state on an input symbol.
     * @param dfa The DFA to advance.
     * @param state The current state.
     * @param symbol The input symbol.
     * @return The next state if a transition exists, otherwise std::nullopt.
     */
    [[nodiscard]] static std::optional<State_t> advance(const Dfa& dfa, State_t state, char symbol);

    /**
     * @brief Checks if a state is an accept state and returns its token if so.
     * @param dfa The DFA to check.
     * @param state The state to check.
     * @return The associated token if the state is accepting, otherwise std::nullopt.
     */
    [[nodiscard]] static std::optional<Token> has_accept_token(const Dfa& dfa, State_t state);

private:
    State_t init_state_;

    Transitions_t transitions_;

    Accept_states_t accept_states_;
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_DFA_HPP
