#ifndef LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_REGEX_HPP
#define LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_REGEX_HPP

#include "lexer/nfa/builder.hpp"

namespace lexer::regex
{
class Regex
{
public:
    virtual ~Regex() = default;

    [[nodiscard]] virtual nfa::Builder to_nfa() const = 0;
};

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_REGEX_HPP
