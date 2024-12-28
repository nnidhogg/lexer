#include "lexer/regex/choice.hpp"

#include <algorithm>

namespace lexer::regex
{
nfa::Builder Choice::to_nfa() const
{
    /**
     * Connect all NFAs with ε transitions into a default constructed empty NFA.
     *
     *     / --ε--> (q1)
     * (q0) ---ε--> (q2)
     *     \ --ε--> (q3)
     */
    nfa::Builder nfa;

    std::ranges::for_each(regexes_, [&nfa](const auto& regex) { nfa = nfa.merge(regex->to_nfa()); });

    return nfa;
}

} // namespace lexer::regex
