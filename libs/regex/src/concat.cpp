#include "lexer/regex/concat.hpp"

#include <algorithm>
#include <ranges>

namespace lexer::regex
{
nfa::Builder Concat::to_nfa() const
{
    /**
     * Concatenate all NFAs with ε transitions in sequence.
     *
     * (q0) --ε--> (q1) --ε--> (q2) --ε--> (q3)
     */
    nfa::Builder nfa{regexes_.front()->to_nfa()};

    std::ranges::for_each(
            regexes_ | std::views::drop(1), [&nfa](const auto& regex) { nfa = nfa.append(regex->to_nfa()); });

    return nfa;
}

} // namespace lexer::regex
