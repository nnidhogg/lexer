#ifndef LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_SIMULATION_HPP
#define LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_SIMULATION_HPP

#include <optional>

#include "lexer/common/concepts.hpp"
#include "lexer/nfa/nfa.hpp"

namespace lexer::nfa::tools
{
class Simulation
{
public:
    using Result_t = std::pair<std::optional<Token>, std::size_t>;

    template <common::concepts::Iterator Iterator>
    [[nodiscard]] static Result_t run(const Nfa& nfa, Iterator begin, Iterator end)
    {
        if (begin == end)
        {
            return {std::nullopt, 0};
        }

        auto states{Nfa::epsilon_closure(nfa, {nfa.init_state()})};

        Result_t result{Nfa::find_token(nfa, states), 0};

        for (Iterator current = begin; current != end && !states.empty(); ++current)
        {
            if (states = Nfa::advance(nfa, states, *current); !states.empty())
            {
                if (const auto token = Nfa::find_token(nfa, states))
                {
                    result = {token, std::distance(begin, current) + 1};
                }
            }
        }

        return result;
    }

    template <common::concepts::Iterable Container>
    [[nodiscard]] static auto run(const Nfa& nfa, const Container& container)
    {
        return run(nfa, std::begin(container), std::end(container));
    }
};

} // namespace lexer::nfa::tools

#endif // LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_SIMULATION_HPP
