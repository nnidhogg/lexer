#include "lexer/regex/any_of.hpp"

#include <algorithm>

namespace lexer::regex
{
nfa::Builder Any_of::to_nfa() const
{
    /**
     * Creates a transition for all symbols in set to the same accept state.
     *
     *      / --s[0]--> \
     *     / ---s[1]---> \
     * (q0) ---- ... ---> (q1)
     *     \ ---s[n]---> /
     */
    nfa::Builder nfa;

    const auto accept_state{nfa.next_state()};

    std::ranges::for_each(set_.symbols(), [&nfa, accept_state](const char symbol) {
        nfa.add_transition(nfa.init_state(), nfa::Label{symbol}, accept_state);
    });

    nfa.add_accept_state(accept_state);

    return nfa;
}

} // namespace lexer::regex
