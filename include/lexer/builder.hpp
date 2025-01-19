#ifndef LEXER_INCLUDE_LEXER_BUILDER_HPP
#define LEXER_INCLUDE_LEXER_BUILDER_HPP

#include <memory>

#include "lexer.hpp"
#include "lexer/dfa/dfa.hpp"
#include "lexer/nfa/builder.hpp"
#include "lexer/regex/regex.hpp"

namespace lexer
{
class Builder
{
public:
    template <typename T>
    void add_token(const std::shared_ptr<const regex::Regex>& regex, T token, const std::size_t priority)
    {
        static_assert(std::is_enum_v<T> || std::is_integral_v<T>, "T must be an integral type or an enum");

        if (!regex)
        {
            throw std::runtime_error("Invalid regex pointer");
        }

        add_token(regex, {static_cast<std::size_t>(token), priority});
    }

    Lexer build() const;

private:
    void add_token(const std::shared_ptr<const regex::Regex>& regex, const nfa::Token& token);

    [[nodiscard]] static dfa::Dfa subset_construction(const nfa::Nfa& nfa);

    nfa::Builder nfa_;
};

} // namespace lexer

#endif // LEXER_INCLUDE_LEXER_BUILDER_HPP
