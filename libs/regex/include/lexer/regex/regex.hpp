#ifndef LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_REGEX_HPP
#define LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_REGEX_HPP

#include "lexer/nfa/builder.hpp"

namespace lexer::regex
{
/**
 * @brief Abstract base class for all regex nodes.
 *
 * Provides an interface for converting regex nodes to NFA builders.
 */
class Regex
{
public:
    virtual ~Regex() = default;

    /**
     * @brief Converts this regex node to an NFA builder.
     * @return NFA builder representing this regex.
     */
    [[nodiscard]] virtual nfa::Builder to_nfa() const = 0;
};

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_REGEX_HPP
