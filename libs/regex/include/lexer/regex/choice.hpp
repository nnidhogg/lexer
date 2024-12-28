#ifndef LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_CHOICE_HPP
#define LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_CHOICE_HPP

#include <memory>
#include <vector>

#include "regex.hpp"

namespace lexer::regex
{
class Choice final : public Regex
{
public:
    template <typename... Args>
    static std::shared_ptr<Choice> create(Args&&... args)
    {
        return std::shared_ptr<Choice>(new Choice(std::forward<Args>(args)...));
    }

    Choice(const Choice&) = delete;
    Choice& operator=(const Choice&) = delete;

    Choice(Choice&&) = delete;
    Choice& operator=(Choice&&) = delete;

    [[nodiscard]] nfa::Builder to_nfa() const override;

private:
    template <typename... Args>
    explicit Choice(Args&&... args) : regexes_{std::forward<Args>(args)...}
    {
        static_assert(sizeof...(Args) > 0, "Constructor requires at least one argument");
    }

    std::vector<std::shared_ptr<const Regex>> regexes_;
};

template <typename... Args>
std::shared_ptr<const Regex> choice(Args&&... args)
{
    return Choice::create(std::forward<Args>(args)...);
}

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_CHOICE_HPP
