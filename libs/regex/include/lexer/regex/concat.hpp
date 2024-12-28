#ifndef LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_CONCAT_HPP
#define LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_CONCAT_HPP

#include <memory>
#include <vector>

#include "regex.hpp"

namespace lexer::regex
{
class Concat final : public Regex
{
public:
    template <typename... Args>
    static std::shared_ptr<Concat> create(Args&&... args)
    {
        return std::shared_ptr<Concat>(new Concat(std::forward<Args>(args)...));
    }

    Concat(const Concat&) = delete;
    Concat& operator=(const Concat&) = delete;

    Concat(Concat&&) = delete;
    Concat& operator=(Concat&&) = delete;

    [[nodiscard]] nfa::Builder to_nfa() const override;

private:
    template <typename... Args>
    explicit Concat(Args&&... args) : regexes_{std::forward<Args>(args)...}
    {
        static_assert(sizeof...(Args) > 0, "Constructor requires at least one argument");
    }

    std::vector<std::shared_ptr<const Regex>> regexes_;
};

template <typename... Args>
std::shared_ptr<const Regex> concat(Args&&... args)
{
    return Concat::create(std::forward<Args>(args)...);
}

} // namespace lexer::regex

#endif // LEXER_LIBS_REGEX_INCLUDE_LEXER_REGEX_CONCAT_HPP
