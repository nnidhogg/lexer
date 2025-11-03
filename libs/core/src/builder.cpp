#include "lexer/core/builder.hpp"

#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <numeric>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "lexer/dfa/builder.hpp"

namespace
{
struct Hash
{
    std::size_t operator()(const lexer::nfa::Nfa::States_t& states) const noexcept
    {
        const auto hash{[](auto seed, const auto element) { return boost::hash_combine(seed, element), seed; }};

        return std::accumulate(states.cbegin(), states.cend(), static_cast<std::size_t>(0), hash);
    }
};

auto build_symbol_table(const lexer::nfa::Nfa& nfa)
{
    std::unordered_map<size_t, std::unordered_set<lexer::nfa::Label::Symbol_t>> result;

    const auto filter{[](const auto& pair) { return pair.second.is_symbol(); }};

    auto view{nfa.transitions() | std::views::keys | std::views::filter(filter)};

    std::ranges::for_each(view, [&result](const auto& pair) { result[pair.first].insert(pair.second.symbol()); });

    return result;
}

} // namespace

namespace lexer::core
{
Lexer Builder::build() const
{
    return Lexer{dfa()};
}

nfa::Nfa Builder::nfa() const
{
    return nfa_.build();
}

dfa::Dfa Builder::dfa() const
{
    return subset_construction(nfa());
}

void Builder::add_token(const std::shared_ptr<const regex::Regex>& regex, const nfa::Token& token)
{
    nfa_ = nfa_.merge(regex->to_nfa().set_accept_token(token));
}

dfa::Dfa Builder::subset_construction(const nfa::Nfa& nfa)
{
    dfa::Builder dfa;

    const auto symbol_table{build_symbol_table(nfa)};

    const auto initial_states{nfa::Nfa::epsilon_closure(nfa, {nfa.init_state()})};

    std::unordered_map<nfa::Nfa::States_t, dfa::Dfa::State_t, Hash> nfa_dfa_map{{initial_states, dfa.init_state()}};

    std::queue<nfa::Nfa::States_t> nfa_queue{{initial_states}};

    while (!nfa_queue.empty())
    {
        const auto nfa_states{nfa_queue.front()};

        nfa_queue.pop();

        const auto dfa_state{nfa_dfa_map.at(nfa_states)};

        if (const auto token = nfa::Nfa::has_accept_token(nfa, nfa_states); token)
        {
            dfa.add_accept_state(dfa_state, dfa::Token{token->id()});
        }

        const auto filter{[&symbol_table](const auto state) { return symbol_table.contains(state); }};

        const auto transform{[&symbol_table](const auto state) { return symbol_table.at(state); }};

        auto view{nfa_states | std::views::filter(filter) | std::views::transform(transform) | std::views::join};

        std::ranges::for_each(view, [&](const auto symbol) {
            const auto next_states{nfa::Nfa::advance(nfa, nfa_states, symbol)};

            if (!next_states.empty() && nfa_dfa_map.emplace(next_states, dfa.next_state()).second)
            {
                nfa_queue.push(next_states);
            }

            dfa.add_transition(dfa_state, dfa::Label{symbol}, nfa_dfa_map.at(next_states));
        });
    }

    return dfa.build();
}

} // namespace lexer::core
