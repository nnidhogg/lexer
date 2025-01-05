#ifndef LEXER_INCLUDE_LEXER_LEXER_HPP
#define LEXER_INCLUDE_LEXER_LEXER_HPP

#include <optional>

#include "lexer/common/concepts.hpp"
#include "lexer/dfa/dfa.hpp"
#include "lexer/dfa/tools/simulation.hpp"

namespace lexer
{
class Lexer
{
public:
    explicit Lexer(dfa::Dfa dfa);

    template <typename T>
    using Result_t = std::pair<std::optional<T>, std::size_t>;

    template <typename T, common::concepts::Iterator Iterator>
    [[nodiscard]] Result_t<T> tokenize(Iterator begin, Iterator end) const
    {
        static_assert(std::is_enum_v<T> || std::is_integral_v<T>, "T must be an integral type or an enum");

        auto [token, offset] = dfa::tools::Simulation::run(dfa_, begin, end);

        return {token ? std::optional<T>{static_cast<T>(token->id())} : std::nullopt, offset};
    }

    template <typename T, common::concepts::Iterable Container>
    [[nodiscard]] auto tokenize(const Container& container) const
    {
        return tokenize<T>(std::begin(container), std::end(container));
    }

    const dfa::Dfa& dfa() const;

private:
    dfa::Dfa dfa_;
};

} // namespace lexer

#endif // LEXER_INCLUDE_LEXER_LEXER_HPP
