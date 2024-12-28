#ifndef LEXER_LIBS_DFA_TEST_TOOLS_INCLUDE_SIMULATION_HPP
#define LEXER_LIBS_DFA_TEST_TOOLS_INCLUDE_SIMULATION_HPP

#include <optional>

#include "lexer/common/concepts.hpp"
#include "lexer/dfa/dfa.hpp"

namespace lexer::dfa::tools
{
class Simulation
{
public:
    using Result_t = std::pair<std::optional<Token>, std::size_t>;

    template <common::concepts::Iterator Iterator>
    [[nodiscard]] static Result_t run(const Dfa& dfa, Iterator begin, Iterator end)
    {
        if (begin == end)
        {
            return {std::nullopt, 0};
        }

        std::optional state{dfa.init_state()};

        Result_t result{Dfa::find_token(dfa, *state), 0};

        for (Iterator current = begin; current != end && state; ++current)
        {
            if (state = Dfa::advance(dfa, *state, *current); state)
            {
                if (const auto token = Dfa::find_token(dfa, *state))
                {
                    result = {token, std::distance(begin, current) + 1};
                }
            }
        }

        return result;
    }

    template <common::concepts::Iterable Container>
    [[nodiscard]] static auto run(const Dfa& dfa, const Container& container)
    {
        return run(dfa, std::begin(container), std::end(container));
    }
};

} // namespace lexer::dfa::tools

#endif // LEXER_LIBS_DFA_TEST_TOOLS_INCLUDE_SIMULATION_HPP
