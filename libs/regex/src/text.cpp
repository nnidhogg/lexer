#include "lexer/regex/text.hpp"

#include <algorithm>

namespace lexer::regex
{
nfa::Builder Text::to_nfa() const
{
    /**
     * Creates a sequence of transitions for each symbol in text.
     *
     * (q0) --s[0]--> (q1) --s[1]--> (q2) ... --s[n]--> (qn)
     */
    nfa::Builder nfa;

    const auto accept_state{std::ranges::fold_left(text_, nfa.init_state(), [&nfa](const auto from, const char symbol) {
        const auto to{nfa.next_state()};
        nfa.add_transition(from, nfa::Label{symbol}, to);
        return to;
    })};

    nfa.add_accept_state(accept_state);

    return nfa;
}

} // namespace lexer::regex
