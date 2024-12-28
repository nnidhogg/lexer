#ifndef LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP
#define LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP

#include <filesystem>
#include <string>

#include "lexer/nfa/label.hpp"
#include "lexer/nfa/nfa.hpp"

namespace lexer::nfa::tools
{
/**
 * @brief Utility class for exporting NFA objects to Graphviz DOT format and files.
 *
 * Provides static methods to generate DOT representations and write them to files for visualization.
 */
class Graphviz
{
public:
    /**
     * @brief Writes the DOT representation of an NFA to a file.
     * @param nfa The NFA to export.
     * @param path The file path to write the DOT output to.
     */
    static void to_file(const Nfa& nfa, const std::filesystem::path& path);

    /**
     * @brief Generates the DOT representation of an NFA as a string.
     * @param nfa The NFA to export.
     * @return The DOT format string representing the NFA.
     */
    [[nodiscard]] static std::string to_dot(const Nfa& nfa);

private:
    /**
     * @brief Creates a DOT label for an NFA transition label.
     * @param label The NFA label.
     * @return The DOT label string.
     */
    [[nodiscard]] static std::string create_label(const Label& label);
};

} // namespace lexer::nfa::tools

#endif // LEXER_LIBS_NFA_TEST_TOOLS_INCLUDE_GRAPHVIZ_HPP
