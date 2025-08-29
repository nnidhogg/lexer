#include "lexer/dfa/dfa.hpp"

#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <ranges>

namespace lexer::dfa
{
std::size_t Dfa::Hash::operator()(const Key_t& key) const noexcept
{
    std::size_t seed{};
    return boost::hash_combine(seed, key.first), boost::hash_combine(seed, std::hash<Label>{}(key.second)), seed;
}

Dfa::Dfa(const State_t init_state, Transitions_t transitions, Accept_states_t accept_states)
    : init_state_{init_state}, transitions_{std::move(transitions)}, accept_states_{std::move(accept_states)}
{}

Dfa::State_t Dfa::init_state() const
{
    return init_state_;
}

const Dfa::Transitions_t& Dfa::transitions() const
{
    return transitions_;
}

const Dfa::Accept_states_t& Dfa::accept_states() const
{
    return accept_states_;
}

std::optional<Dfa::State_t> Dfa::advance(const Dfa& dfa, const State_t state, const char symbol)
{
    const std::pair key{state, Label{symbol}};

    return dfa.transitions().contains(key) ? std::optional{dfa.transitions().at(key)} : std::nullopt;
}

std::optional<Token> Dfa::has_accept_token(const Dfa& dfa, const State_t state)
{
    return dfa.accept_states().contains(state) ? std::optional{dfa.accept_states().at(state)} : std::nullopt;
}

} // namespace lexer::dfa
