#include "lexer/regex/repeat.hpp"

#include <algorithm>
#include <ranges>

namespace lexer::regex
{
Repeat::Repeat(const Variant_t& variant, std::shared_ptr<const Regex> regex)
    : variant_{variant}, regex_{std::move(regex)}
{}

std::shared_ptr<Repeat> Repeat::kleene(std::shared_ptr<const Regex> regex)
{
    return std::shared_ptr<Repeat>(new Repeat(Kleene{}, std::move(regex)));
}

std::shared_ptr<Repeat> Repeat::plus(std::shared_ptr<const Regex> regex)
{
    return std::shared_ptr<Repeat>(new Repeat(Plus{}, std::move(regex)));
}

std::shared_ptr<Repeat> Repeat::optional(std::shared_ptr<const Regex> regex)
{
    return std::shared_ptr<Repeat>(new Repeat(Optional{}, std::move(regex)));
}

std::shared_ptr<Repeat> Repeat::exact(std::shared_ptr<const Regex> regex, const std::size_t count)
{
    return std::shared_ptr<Repeat>(new Repeat(Exact{count}, std::move(regex)));
}

std::shared_ptr<Repeat> Repeat::at_least(std::shared_ptr<const Regex> regex, const std::size_t min)
{
    return std::shared_ptr<Repeat>(new Repeat(At_least{min}, std::move(regex)));
}

std::shared_ptr<Repeat> Repeat::range(std::shared_ptr<const Regex> regex, const std::size_t min, const std::size_t max)
{
    return std::shared_ptr<Repeat>(new Repeat(Range{min, max}, std::move(regex)));
}

nfa::Builder Repeat::to_nfa() const
{
    return std::visit(
            [this]<typename T>(const T& value) {
                if constexpr (std::is_same_v<T, Kleene>)
                {
                    return to_kleene();
                }
                else if constexpr (std::is_same_v<T, Plus>)
                {
                    return to_plus();
                }
                else if constexpr (std::is_same_v<T, Optional>)
                {
                    return to_optional();
                }
                else if constexpr (std::is_same_v<T, Exact>)
                {
                    return to_exact(value.count);
                }
                else if constexpr (std::is_same_v<T, At_least>)
                {
                    return to_at_least(value.min);
                }
                else if constexpr (std::is_same_v<T, Range>)
                {
                    return to_range(value.min, value.max);
                }
            },
            variant_);
}

[[nodiscard]] nfa::Builder Repeat::to_kleene() const
{
    /**
     * Matches zero or more occurrences of a sub-pattern.
     *
     *       / <--------ε-------- \
     *      /                      \
     * ((S)) --ε--> ((regex)) --ε-->
     */
    nfa::Builder S;

    S = S.merge(regex_->to_nfa());

    std::ranges::for_each(
            S.accept_states(), [&S](const auto& pair) { S.add_epsilon_transition(pair.first, S.init_state()); });

    S.add_accept_state(S.init_state());

    return S;
}

[[nodiscard]] nfa::Builder Repeat::to_plus() const
{
    /**
     * Matches one or more occurrences of a sub-pattern.
     *
     *     / <--------ε-------- \
     *    /                      \
     * (S) --ε--> ((regex)) --ε-->
     */
    nfa::Builder S;

    S = S.merge(regex_->to_nfa());

    std::ranges::for_each(
            S.accept_states(), [&S](const auto& pair) { S.add_epsilon_transition(pair.first, S.init_state()); });

    return S;
}

[[nodiscard]] nfa::Builder Repeat::to_optional() const
{
    /**
     * Matches zero or one occurrences of a sub-pattern.
     *
     * ((S)) --ε--> ((regex))
     */
    nfa::Builder S;

    S = S.merge(regex_->to_nfa());

    S.add_accept_state(S.init_state());

    return S;
}

[[nodiscard]] nfa::Builder Repeat::to_exact(const std::size_t count) const
{
    /**
     * Matches an exact number of occurrences of a sub-pattern.
     *
     * (S) --ε--> ... --ε--> ((regex n))
     */
    nfa::Builder S;

    S.add_accept_state(S.init_state());

    std::ranges::for_each(std::ranges::iota_view(static_cast<std::size_t>(0), count), [this, &S](auto) {
        S = S.append(regex_->to_nfa());
    });

    return S;
}

[[nodiscard]] nfa::Builder Repeat::to_at_least(const std::size_t min) const
{
    /**
     * Matches a range of occurrences of a sub-pattern.
     *
     *                 / <-----ε----- \
     *                /                \
     * (S) --ε--> ... ((regex n)) --ε-->
     */
    nfa::Builder S;

    S.add_accept_state(S.init_state());

    std::ranges::for_each(
            std::views::iota(static_cast<std::size_t>(1), min), [this, &S](auto) { S = S.append(regex_->to_nfa()); });

    auto F{regex_->to_nfa()};

    std::ranges::for_each(std::views::keys(F.accept_states()), [&F](const auto state) {
        F.add_epsilon_transition(state, F.init_state());
    });

    return S.append(F);
}

[[nodiscard]] nfa::Builder Repeat::to_range(const std::size_t min, const std::size_t max) const
{
    /**
     * Matches a range of occurrences of a sub-pattern.
     *
     * (S) --ε--> ... (regex n) --ε--> ... --ε--> ((regex m))
     *                         \          \                 /
     *                          \          \ ------ε-----> /
     *                           \                        /
     *                            \ ---------ε---------> /
     */
    nfa::Builder S;

    S.add_accept_state(S.init_state());

    std::ranges::for_each(
            std::views::iota(static_cast<std::size_t>(0), min), [this, &S](auto) { S = S.append(regex_->to_nfa()); });

    nfa::Nfa::States_t pending;

    std::ranges::for_each(std::views::iota(min, max), [this, &S, &pending](auto) {
        std::ranges::copy(std::views::keys(S.accept_states()), std::inserter(pending, pending.end()));
        S = S.append(regex_->to_nfa());
    });

    std::ranges::for_each(pending, [&S](const auto pending_state) {
        std::ranges::for_each(std::views::keys(S.accept_states()), [&S, pending_state](const auto accept_state) {
            S.add_epsilon_transition(pending_state, accept_state);
        });
    });

    return S;
}

std::shared_ptr<const Regex> kleene(std::shared_ptr<const Regex> regex)
{
    return Repeat::kleene(std::move(regex));
}

std::shared_ptr<const Regex> plus(std::shared_ptr<const Regex> regex)
{
    return Repeat::plus(std::move(regex));
}

std::shared_ptr<const Regex> optional(std::shared_ptr<const Regex> regex)
{
    return Repeat::optional(std::move(regex));
}

std::shared_ptr<const Regex> exact(std::shared_ptr<const Regex> regex, const std::size_t count)
{
    return Repeat::exact(std::move(regex), count);
}

std::shared_ptr<const Regex> at_least(std::shared_ptr<const Regex> regex, const std::size_t min)
{
    return Repeat::at_least(std::move(regex), min);
}

std::shared_ptr<const Regex> range(std::shared_ptr<const Regex> regex, const std::size_t min, const std::size_t max)
{
    return Repeat::range(std::move(regex), min, max);
}

} // namespace lexer::regex
