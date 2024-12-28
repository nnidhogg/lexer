#ifndef LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_NFA_HPP
#define LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_NFA_HPP

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "label.hpp"
#include "token.hpp"

namespace lexer::nfa
{
/**
 * @brief Represents a non-deterministic finite automaton (NFA).
 *
 * Provides methods for querying states, transitions, and accept states, as well as advancing the NFA and computing
 * epsilon closures.
 */
class Nfa
{
    using Key_t = std::pair<std::size_t, Label>;

    struct Hash
    {
        std::size_t operator()(const Key_t& key) const noexcept;
    };

public:
    using State_t = std::size_t;

    using States_t = std::unordered_set<State_t>;

    using Transitions_t = std::unordered_map<Key_t, States_t, Hash>;

    using Accept_states_t = std::unordered_map<State_t, std::optional<Token>>;

    /**
     * @brief Constructs an NFA with the given initial state, transitions, and accept states.
     * @param init_state The initial state of the NFA.
     * @param transitions The transition table.
     * @param accept_states The accept states and their associated tokens (optional).
     */
    Nfa(State_t init_state, Transitions_t transitions, Accept_states_t accept_states);

    /**
     * @brief Returns the initial state of the NFA.
     * @return The initial state identifier.
     */
    [[nodiscard]] State_t init_state() const;

    /**
     * @brief Returns the transition table of the NFA.
     * @return Reference to the transitions map.
     */
    [[nodiscard]] const Transitions_t& transitions() const;

    /**
     * @brief Returns the accept states and their associated tokens.
     * @return Reference to the accept states map.
     */
    [[nodiscard]] const Accept_states_t& accept_states() const;

    /**
     * @brief Computes the epsilon closure of a set of states in the NFA.
     * @param nfa The NFA to operate on.
     * @param states The set of states to compute the closure for.
     * @return The set of states reachable via epsilon transitions.
     */
    [[nodiscard]] static States_t epsilon_closure(const Nfa& nfa, const States_t& states);

    /**
     * @brief Advances the NFA from a set of states on an input symbol.
     * @param nfa The NFA to advance.
     * @param states The current set of states.
     * @param symbol The input symbol.
     * @return The set of next states reachable on the symbol.
     */
    [[nodiscard]] static States_t advance(const Nfa& nfa, const States_t& states, char symbol);

    /**
     * @brief Checks if any state in the set is an accept state and returns its token if so.
     * @param nfa The NFA to check.
     * @param states The set of states to check.
     * @return The associated token if any state is accepting, otherwise std::nullopt.
     */
    [[nodiscard]] static std::optional<Token> has_accept_token(const Nfa& nfa, const States_t& states);

private:
    State_t init_state_;

    Transitions_t transitions_;

    Accept_states_t accept_states_;
};

} // namespace lexer::nfa

#endif // LEXER_LIBS_NFA_INCLUDE_LEXER_NFA_NFA_HPP
