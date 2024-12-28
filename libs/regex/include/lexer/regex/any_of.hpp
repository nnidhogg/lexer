#ifndef LEXER_LIBS_REGEX_INCLUDE_ANY_HPP
#define LEXER_LIBS_REGEX_INCLUDE_ANY_HPP

#include <memory>

#include "regex.hpp"
#include "set.hpp"

namespace lexer::regex
{
class Any_of final : public Regex
{
public:
    template <typename T, typename = std::enable_if_t<std::is_constructible_v<Set, T>>>
    static std::shared_ptr<Any_of> create(T&& chars)
    {
        return std::shared_ptr<Any_of>(new Any_of(std::forward<T>(chars)));
    }

    Any_of(const Any_of&) = delete;
    Any_of& operator=(const Any_of&) = delete;

    Any_of(Any_of&&) = delete;
    Any_of& operator=(Any_of&&) = delete;

    [[nodiscard]] nfa::Builder to_nfa() const override;

private:
    template <typename T>
    explicit Any_of(T&& chars) : set_{std::forward<T>(chars)}
    {}

    Set set_;
};

template <typename T>
std::shared_ptr<const Regex> any_of(T&& chars)
{
    return Any_of::create(std::forward<T>(chars));
}

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_ANY_HPP
