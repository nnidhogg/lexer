#ifndef LEXER_LIBS_DFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP
#define LEXER_LIBS_DFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP

#include <filesystem>
#include <string>

#include "lexer/dfa/dfa.hpp"
#include "lexer/dfa/label.hpp"

namespace lexer::dfa::tools
{
class Graphviz
{
public:
    static void to_file(const Dfa& dfa, const std::filesystem::path& path);

    [[nodiscard]] static std::string to_dot(const Dfa& dfa);

private:
    [[nodiscard]] static std::string create_label(const Label& label);
};

} // namespace lexer::dfa::tools

#endif // LEXER_LIBS_DFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP
