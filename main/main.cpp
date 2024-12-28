#include <filesystem>
#include <fstream>
#include <iostream>
#include <lexer/nfa/tools/graphviz.hpp>
#include <lexer/regex/any_of.hpp>
#include <lexer/regex/choice.hpp>
#include <lexer/regex/concat.hpp>
#include <lexer/regex/repeat.hpp>
#include <lexer/regex/text.hpp>
#include <string_view>

using namespace lexer::nfa;
using namespace lexer::nfa::tools;
using namespace lexer::regex;

namespace
{
void dot_to_file(const Nfa& nfa, const std::string& file_name)
{
    const std::filesystem::path dot_path{"../debug/" + file_name + ".dot"};
    const std::filesystem::path svg_path{"../debug/" + file_name + ".svg"};

    Graphviz::to_file(nfa, dot_path);

    const std::string command{"dot -Tsvg " + dot_path.string() + " -o " + svg_path.string()};

    const auto error_code = std::system(command.c_str());

    std::cout << command << " exited with " << error_code << std::endl;
}

} // namespace

int main(int, char**)
{
    // Define literals for identifiers 7.2.3
    const auto identifier = concat(any_of(Set::alpha() + '_'), kleene(any_of(Set::alphanum() + '_')));

    dot_to_file(identifier->to_nfa().build(), "identifier");

    // Define literals for integer literals 7.2.6.1
    const auto integer_literal = plus(any_of(Set::digits()));

    dot_to_file(integer_literal->to_nfa().build(), "integer_literal");

    // Define literals for string literals 7.2.6.3
    const auto string_literal = concat(text("\""), kleene(any_of(Set::printable())), text("\""));

    dot_to_file(string_literal->to_nfa().build(), "string_literal");

    const auto wide_string_literal = concat(text("L\""), kleene(any_of(Set::printable())), text("\""));

    dot_to_file(wide_string_literal->to_nfa().build(), "wide_string_literal");

    // Define literals for character literals
    const auto escaped_quote = concat(text("\\"), text("'"));

    const auto character_literal =
            concat(text("'"), choice(any_of(Set::alphanum() + '\\' + '_'), escaped_quote), text("'"));

    dot_to_file(character_literal->to_nfa().build(), "character_literal");

    const auto wide_character_literal =
            concat(text("L'"), choice(any_of(Set::alphanum() + '\\' + '_'), escaped_quote), text("'"));

    dot_to_file(wide_character_literal->to_nfa().build(), "wide_character_literal");

    // Define literals for fixed-point literals
    const auto fixed_pt_literal = concat(plus(any_of(Set::digits())), text("."), plus(any_of(Set::digits())));

    dot_to_file(fixed_pt_literal->to_nfa().build(), "fixed_pt_literal");

    // Define literals for floating-point literals
    const auto sign_part{choice(text("+"), text("-"))};

    const auto exponent_part{concat(choice(text("e"), text("E")), optional(sign_part), plus(any_of(Set::digits())))};

    const auto integer_then_decimal{concat(plus(any_of(Set::digits())), text("."), kleene(any_of(Set::digits())))};

    const auto decimal_then_digits{concat(text("."), plus(any_of(Set::digits())))};

    const auto fraction_part{choice(integer_then_decimal, decimal_then_digits)};

    const auto floating_pt_literal{concat(optional(sign_part), fraction_part, optional(exponent_part))};

    dot_to_file(floating_pt_literal->to_nfa().build(), "floating_pt_literal");

    // Matches '//' followed by any characters except newline
    const auto single_line_comment = concat(text("//"), kleene(any_of(Set::printable() - '\n')));

    dot_to_file(single_line_comment->to_nfa().build(), "single_line_comment");

    // Matches '/*' followed by any characters until '*/'
    const auto multi_line_comment = concat(text("/*"), kleene(any_of(Set::printable())), text("*/"));

    dot_to_file(multi_line_comment->to_nfa().build(), "multi_line_comment");

    return 0;
}
