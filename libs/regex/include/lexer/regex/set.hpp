#ifndef LEXER_LIBS_REGEX_INCLUDE_SET_HPP
#define LEXER_LIBS_REGEX_INCLUDE_SET_HPP

#include <initializer_list>
#include <unordered_set>

namespace lexer::regex
{
class Set
{
public:
    using Symbol_t = char;

    using Symbols_t = std::unordered_set<Symbol_t>;

    Set() = default;

    Set(std::initializer_list<Symbol_t> symbols);

    explicit Set(const Symbols_t& symbols);

    explicit Set(Symbols_t&& symbols);

    const Symbols_t& symbols() const;

    static Set from(Symbol_t s);

    static Set from(std::initializer_list<Symbol_t> symbols);

    static Set range(Symbol_t start, Symbol_t end);

    static Set digits();

    static Set alpha();

    static Set alphanum();

    static Set printable();

    static Set escape();

    static Set all();

    Set& operator+=(const Set& other);

    Set& operator+=(Symbol_t s);

    Set& operator-=(const Set& other);

    Set& operator-=(Symbol_t s);

    friend Set operator+(Set lhs, const Set& rhs);

    friend Set operator+(Set lhs, Symbol_t s);

    friend Set operator-(Set lhs, const Set& rhs);

    friend Set operator-(Set lhs, Symbol_t s);

    friend Set operator+(Symbol_t s, Set rhs);

private:
    Symbols_t symbols_;
};

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_SET_HPP
