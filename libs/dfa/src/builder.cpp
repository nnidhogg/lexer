#include "lexer/dfa/builder.hpp"

#include <algorithm>

namespace lexer::dfa
{
Builder::Builder() : init_state_{0}, next_state_{1}
{}

Builder::Builder(
        const std::size_t init_state, const std::size_t next_state, Dfa::Transitions_t transitions,
        Dfa::Accept_states_t accept_states)
    : init_state_{init_state}
    , next_state_{next_state}
    , transitions_{std::move(transitions)}
    , accept_states_{std::move(accept_states)}
{}

std::size_t Builder::init_state() const
{
    return init_state_;
}

std::size_t Builder::next_state() const
{
    return next_state_;
}

const Dfa::Transitions_t& Builder::transitions() const
{
    return transitions_;
}

const Dfa::Accept_states_t& Builder::accept_states() const
{
    return accept_states_;
}

std::size_t Builder::step_state()
{
    return next_state_++;
}

void Builder::add_transition(const std::size_t from, const Label& label, const std::size_t to)
{
    transitions_[{from, label}] = to;
}

void Builder::add_accept_state(const std::size_t accept_state, const Token& token)
{
    accept_states_.emplace(accept_state, token);
}

Dfa Builder::build() const
{
    return {init_state_, transitions_, accept_states_};
}

} // namespace lexer::dfa
