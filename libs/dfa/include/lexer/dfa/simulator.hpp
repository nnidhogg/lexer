#ifndef LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_SIMULATOR_HPP
#define LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_SIMULATOR_HPP

#include <optional>

#include "lexer/common/concepts.hpp"
#include "lexer/dfa/dfa.hpp"

namespace lexer::dfa
{
/**
 * @brief Simulator for running a DFA over an input sequence.
 *
 * Provides static methods to simulate DFA execution over iterators or containers.
 */
class Simulator
{
public:
    /**
     * @brief The result type: a pair of the matched token (if any) and the length of the match.
     */
    using Result_t = std::pair<std::optional<Token>, std::size_t>;

    /**
     * @brief Runs the DFA simulation over a range defined by iterators.
     * @tparam Iterator Input iterator type.
     * @param dfa The DFA to simulate.
     * @param begin Iterator to the beginning of the input.
     * @param end Iterator to the end of the input.
     * @return A pair containing the matched token (if any) and the length of the match.
     */
    template <common::concepts::Iterator Iterator>
    [[nodiscard]] static Result_t run(const Dfa& dfa, Iterator begin, Iterator end)
    {
        if (begin == end)
        {
            return {std::nullopt, 0};
        }

        std::optional state{dfa.init_state()};

        Result_t result{Dfa::has_accept_token(dfa, *state), 0};

        for (Iterator current = begin; current != end && state; ++current)
        {
            if (state = Dfa::advance(dfa, *state, *current); !state)
            {
                continue;
            }

            if (const auto token = Dfa::has_accept_token(dfa, *state); token)
            {
                result = {token, std::distance(begin, current) + 1};
            }
        }

        return result;
    }

    /**
     * @brief Runs the DFA simulation over a container.
     * @tparam Container The container type (must be iterable).
     * @param dfa The DFA to simulate.
     * @param container The input container.
     * @return A pair containing the matched token (if any) and the length of the match.
     */
    template <common::concepts::Iterable Container>
    [[nodiscard]] static Result_t run(const Dfa& dfa, const Container& container)
    {
        return run(dfa, std::begin(container), std::end(container));
    }
};

} // namespace lexer::dfa

#endif // LEXER_LIBS_DFA_INCLUDE_LEXER_DFA_SIMULATOR_HPP
