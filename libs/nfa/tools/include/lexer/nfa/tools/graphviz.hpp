#ifndef LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP
#define LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP

#include <filesystem>
#include <string>

#include "lexer/nfa/label.hpp"
#include "lexer/nfa/nfa.hpp"

namespace lexer::nfa::tools
{
class Graphviz
{
public:
    static void to_file(const Nfa& nfa, const std::filesystem::path& path);

    [[nodiscard]] static std::string to_dot(const Nfa& nfa);

private:
    [[nodiscard]] static std::string create_label(const Label& label);
};

} // namespace lexer::nfa::tools

#endif // LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP
