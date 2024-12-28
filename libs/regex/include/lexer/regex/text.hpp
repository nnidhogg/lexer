#ifndef LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_TEXT_HPP
#define LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_TEXT_HPP

#include <memory>
#include <string>

#include "regex.hpp"

namespace lexer::regex
{
class Text final : public Regex
{
public:
    template <typename T>
    static std::shared_ptr<Text> create(T&& arg)
    {
        return std::shared_ptr<Text>(new Text(std::forward<T>(arg)));
    }

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;

    Text(Text&&) = delete;
    Text& operator=(Text&&) = delete;

    [[nodiscard]] nfa::Builder to_nfa() const override;

private:
    template <typename T>
    explicit Text(T&& arg) : text_{std::forward<T>(arg)}
    {}

    std::string text_;
};

template <typename T>
std::shared_ptr<const Regex> text(T&& arg)
{
    return Text::create(std::forward<T>(arg));
}

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_TEXT_HPP
