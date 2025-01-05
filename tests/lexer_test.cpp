#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <lexer/dfa/tools/graphviz.hpp>
#include <lexer/nfa/tools/graphviz.hpp>
#include <lexer/regex/any_of.hpp>
#include <lexer/regex/choice.hpp>
#include <lexer/regex/concat.hpp>
#include <lexer/regex/repeat.hpp>
#include <lexer/regex/text.hpp>

#include "lexer/builder.hpp"

using namespace lexer;
using namespace lexer::regex;

class Lexer_test : public testing::Test
{
protected:
    static auto identifier_regex()
    {
        const auto identifier{concat(any_of(Set::alpha() + '_'), kleene(any_of(Set::alphanum() + '_')))};

        return identifier;
    }

    static auto integer_literal_regex()
    {
        const auto integer_literal{plus(any_of(Set::digits()))};

        return integer_literal;
    }

    static auto string_literal_regex()
    {
        const auto string_literal{concat(text("\""), kleene(any_of(Set::printable())), text("\""))};

        return string_literal;
    }

    static auto fixed_point_literal_regex()
    {
        const auto fixed_point_literal{concat(plus(any_of(Set::digits())), text("."), plus(any_of(Set::digits())))};

        return fixed_point_literal;
    }

    static auto floating_point_literal_regex()
    {
        const auto any_digit{any_of(Set::digits())};

        const auto sign_part{choice(text("+"), text("-"))};

        const auto exponent_part{concat(choice(text("e"), text("E")), optional(sign_part), plus(any_digit))};

        const auto leading_digits{concat(plus(any_digit), text("."), kleene(any_digit), optional(exponent_part))};

        const auto leading_decimal{concat(text("."), plus(any_digit), optional(exponent_part))};

        const auto forced_exponent{concat(plus(any_digit), exponent_part)};

        const auto fraction_part{choice(leading_digits, leading_decimal, forced_exponent)};

        const auto floating_point_literal{concat(optional(sign_part), fraction_part)};

        return floating_point_literal;
    }

    static auto wide_string_literal_regex()
    {
        const auto wide_string_literal{
                concat(text("L\""), kleene(any_of(Set::printable() + Set::escape())), text("\""))};

        return wide_string_literal;
    }

    static auto character_literal_regex()
    {
        const auto character_literal{concat(text("'"), any_of(Set::printable() + Set::escape()), text("'"))};

        return character_literal;
    }

    static auto wide_character_literal_regex()
    {
        const auto wide_character_literal{concat(text("L'"), any_of(Set::printable() + Set::escape()), text("'"))};

        return wide_character_literal;
    }

    static auto single_line_comment_regex()
    {
        const auto single_line_comment{concat(text("//"), kleene(any_of(Set::printable() + Set::escape())))};

        return single_line_comment;
    }

    static auto multi_line_comment_regex()
    {
        const auto multi_line_comment{concat(text("/*"), kleene(any_of(Set::printable() + Set::escape())), text("*/"))};

        return multi_line_comment;
    }

    void write_dot(const nfa::Nfa& nfa, const std::string& name) const
    {
        const std::filesystem::path dot_path{debug_path_ / (name + ".dot")};

        nfa::tools::Graphviz::to_file(nfa, dot_path);

        const std::filesystem::path svg_path{debug_path_ / (name + ".svg")};

        dot_to_svg(dot_path, svg_path);
    }

    void write_dot(const dfa::Dfa& dfa, const std::string& name) const
    {
        const std::filesystem::path dot_path{debug_path_ / (name + ".dot")};

        dfa::tools::Graphviz::to_file(dfa, dot_path);

        const std::filesystem::path svg_path{debug_path_ / (name + ".svg")};

        dot_to_svg(dot_path, svg_path);
    }

private:
    static void dot_to_svg(const auto& dot_path, const auto& svg_path)
    {
        const std::string command{"dot -Tsvg " + dot_path.string() + " -o " + svg_path.string()};

        std::cout << command << " exited with " << std::system(command.c_str()) << std::endl;
    }

    std::filesystem::path debug_path_{std::string(SOURCE_DIR) + "/debug/"};
};

TEST_F(Lexer_test, Test_keywords)
{
    enum class Token_type : uint8_t
    {
        Boolean,
        Char,
        String,
        Int8,
        Uint8,
        Int16,
        Uint16,
        Int32,
        Uint32,
        Int64,
        Uint64,
    };

    Builder builder;

    builder.add_token(text("boolean"), Token_type::Boolean, 1);
    builder.add_token(text("char"), Token_type::Char, 1);
    builder.add_token(text("string"), Token_type::String, 1);
    builder.add_token(text("int8"), Token_type::Int8, 1);
    builder.add_token(text("uint8"), Token_type::Uint8, 1);
    builder.add_token(text("int16"), Token_type::Int16, 1);
    builder.add_token(text("uint16"), Token_type::Uint16, 1);
    builder.add_token(text("int32"), Token_type::Int32, 1);
    builder.add_token(text("uint32"), Token_type::Uint32, 1);
    builder.add_token(text("int64"), Token_type::Int64, 1);
    builder.add_token(text("uint64"), Token_type::Uint64, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("boolean"), Result_t(Token_type::Boolean, 7));
    ASSERT_EQ(lexer.tokenize<Token_type>("char"), Result_t(Token_type::Char, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("string"), Result_t(Token_type::String, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("int8"), Result_t(Token_type::Int8, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint8"), Result_t(Token_type::Uint8, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("int16"), Result_t(Token_type::Int16, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint16"), Result_t(Token_type::Uint16, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("int32"), Result_t(Token_type::Int32, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint32"), Result_t(Token_type::Uint32, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("int64"), Result_t(Token_type::Int64, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint64"), Result_t(Token_type::Uint64, 6));
}

TEST_F(Lexer_test, Test_identifier)
{
    enum class Token_type : uint8_t
    {
        Identifier,
    };

    Builder builder;

    builder.add_token(identifier_regex(), Token_type::Identifier, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("variable_name"), Result_t(Token_type::Identifier, 13));
    ASSERT_EQ(lexer.tokenize<Token_type>("_someVar"), Result_t(Token_type::Identifier, 8));
    ASSERT_EQ(lexer.tokenize<Token_type>("MyVariable123"), Result_t(Token_type::Identifier, 13));
    ASSERT_EQ(lexer.tokenize<Token_type>("__Another_var__99"), Result_t(Token_type::Identifier, 17));
    ASSERT_EQ(lexer.tokenize<Token_type>("camelCase"), Result_t(Token_type::Identifier, 9));
    ASSERT_EQ(lexer.tokenize<Token_type>("___"), Result_t(Token_type::Identifier, 3));
}

TEST_F(Lexer_test, Test_integer_literal)
{
    enum class Token_type : uint8_t
    {
        Integer_literal,
    };

    Builder builder;

    builder.add_token(integer_literal_regex(), Token_type::Integer_literal, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("123"), Result_t(Token_type::Integer_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("007"), Result_t(Token_type::Integer_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("1234567890"), Result_t(Token_type::Integer_literal, 10));
    ASSERT_EQ(lexer.tokenize<Token_type>("5"), Result_t(Token_type::Integer_literal, 1));
    ASSERT_EQ(lexer.tokenize<Token_type>("0"), Result_t(Token_type::Integer_literal, 1));
}

TEST_F(Lexer_test, Test_string_literal)
{
    enum class Token_type : uint8_t
    {
        String_literal,
    };

    Builder builder;

    builder.add_token(string_literal_regex(), Token_type::String_literal, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("\"Hello\""), Result_t(Token_type::String_literal, 7));
    ASSERT_EQ(lexer.tokenize<Token_type>("\"\""), Result_t(Token_type::String_literal, 2));
    ASSERT_EQ(lexer.tokenize<Token_type>("\"Hello world\""), Result_t(Token_type::String_literal, 13));
    ASSERT_EQ(lexer.tokenize<Token_type>("\"\\\"Quote\\\"\""), Result_t(Token_type::String_literal, 11));
}

TEST_F(Lexer_test, Test_fixed_point_literal)
{
    enum class Token_type : uint8_t
    {
        Fixed_point_literal,
    };

    Builder builder;

    builder.add_token(fixed_point_literal_regex(), Token_type::Fixed_point_literal, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("1.2"), Result_t(Token_type::Fixed_point_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("3.14"), Result_t(Token_type::Fixed_point_literal, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("123.456"), Result_t(Token_type::Fixed_point_literal, 7));

    ASSERT_EQ(lexer.tokenize<Token_type>("."), Result_t(std::nullopt, 0));
    ASSERT_EQ(lexer.tokenize<Token_type>(".1"), Result_t(std::nullopt, 0));
    ASSERT_EQ(lexer.tokenize<Token_type>("58."), Result_t(std::nullopt, 0));
}

TEST_F(Lexer_test, Test_wide_string_literals)
{
    enum class Token_type : uint8_t
    {
        Wide_string_literal,
    };

    Builder builder;

    builder.add_token(wide_string_literal_regex(), Token_type::Wide_string_literal, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("L\"Hello\""), Result_t(Token_type::Wide_string_literal, 8));
    ASSERT_EQ(lexer.tokenize<Token_type>("L\"\""), Result_t(Token_type::Wide_string_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("L\"Wide world\""), Result_t(Token_type::Wide_string_literal, 13));
    ASSERT_EQ(lexer.tokenize<Token_type>("L\"\\\"Escaped\\\"\""), Result_t(Token_type::Wide_string_literal, 14));
}

TEST_F(Lexer_test, Test_character_literals)
{
    enum class Token_type : uint8_t
    {
        Character_literal,
    };

    Builder builder;

    builder.add_token(character_literal_regex(), Token_type::Character_literal, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("'a'"), Result_t(Token_type::Character_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("' '"), Result_t(Token_type::Character_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("'\n'"), Result_t(Token_type::Character_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("'\''"), Result_t(Token_type::Character_literal, 3));
}

TEST_F(Lexer_test, Test_wide_character_literals)
{
    enum class Token_type : uint8_t
    {
        Wide_character_literal,
    };

    Builder builder;

    builder.add_token(wide_character_literal_regex(), Token_type::Wide_character_literal, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("L'a'"), Result_t(Token_type::Wide_character_literal, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("L' '"), Result_t(Token_type::Wide_character_literal, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("L'\n'"), Result_t(Token_type::Wide_character_literal, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("L'\''"), Result_t(Token_type::Wide_character_literal, 4));
}

TEST_F(Lexer_test, Test_single_line_comments)
{
    enum class Token_type : uint8_t
    {
        Single_line_comment,
    };

    Builder builder;

    builder.add_token(single_line_comment_regex(), Token_type::Single_line_comment, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("// This is a comment"), Result_t(Token_type::Single_line_comment, 20));
    ASSERT_EQ(lexer.tokenize<Token_type>("//"), Result_t(Token_type::Single_line_comment, 2));
    ASSERT_EQ(lexer.tokenize<Token_type>("// @#$%^&*()"), Result_t(Token_type::Single_line_comment, 12));
}

TEST_F(Lexer_test, Test_multi_line_comments)
{
    enum class Token_type : uint8_t
    {
        Multi_line_comment,
    };

    Builder builder;

    builder.add_token(multi_line_comment_regex(), Token_type::Multi_line_comment, 1);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("/* comment */"), Result_t(Token_type::Multi_line_comment, 13));
    ASSERT_EQ(
            lexer.tokenize<Token_type>("/* multi\n   line\n   comment */"),
            Result_t(Token_type::Multi_line_comment, 30));
    ASSERT_EQ(lexer.tokenize<Token_type>("/* start /* nested */ end */"), Result_t(Token_type::Multi_line_comment, 28));
    ASSERT_EQ(lexer.tokenize<Token_type>("/**/"), Result_t(Token_type::Multi_line_comment, 4));
}

TEST_F(Lexer_test, Test_combined)
{
    enum class Token_type : uint8_t
    {
        // Keywords
        Boolean,
        Char,
        String,
        Int8,
        Uint8,
        Int16,
        Uint16,
        Int32,
        Uint32,
        Int64,
        Uint64,

        // Identifier
        Identifier,

        // Literals
        Integer_literal,
        String_literal,
        Wide_string_literal,
        Character_literal,
        Wide_character_literal,
        Fixed_point_literal,
        Floating_point_literal,

        // Comments
        Single_line_comment,
        Multi_line_comment,
    };

    Builder builder;

    builder.add_token(text("boolean"), Token_type::Boolean, 1);
    builder.add_token(text("char"), Token_type::Char, 1);
    builder.add_token(text("string"), Token_type::String, 1);
    builder.add_token(text("int8"), Token_type::Int8, 1);
    builder.add_token(text("uint8"), Token_type::Uint8, 1);
    builder.add_token(text("int16"), Token_type::Int16, 1);
    builder.add_token(text("uint16"), Token_type::Uint16, 1);
    builder.add_token(text("int32"), Token_type::Int32, 1);
    builder.add_token(text("uint32"), Token_type::Uint32, 1);
    builder.add_token(text("int64"), Token_type::Int64, 1);
    builder.add_token(text("uint64"), Token_type::Uint64, 1);

    builder.add_token(identifier_regex(), Token_type::Identifier, 4);

    builder.add_token(integer_literal_regex(), Token_type::Integer_literal, 2);
    builder.add_token(string_literal_regex(), Token_type::String_literal, 2);
    builder.add_token(character_literal_regex(), Token_type::Character_literal, 2);
    builder.add_token(wide_string_literal_regex(), Token_type::Wide_string_literal, 2);
    builder.add_token(wide_character_literal_regex(), Token_type::Wide_character_literal, 2);

    builder.add_token(fixed_point_literal_regex(), Token_type::Fixed_point_literal, 2);
    builder.add_token(floating_point_literal_regex(), Token_type::Floating_point_literal, 3);

    builder.add_token(single_line_comment_regex(), Token_type::Single_line_comment, 0);
    builder.add_token(multi_line_comment_regex(), Token_type::Multi_line_comment, 0);

    const auto lexer{builder.build()};

    using Result_t = Lexer::Result_t<Token_type>;

    ASSERT_EQ(lexer.tokenize<Token_type>("boolean"), Result_t(Token_type::Boolean, 7));
    ASSERT_EQ(lexer.tokenize<Token_type>("char"), Result_t(Token_type::Char, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("string"), Result_t(Token_type::String, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("int8"), Result_t(Token_type::Int8, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint8"), Result_t(Token_type::Uint8, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("int16"), Result_t(Token_type::Int16, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint16"), Result_t(Token_type::Uint16, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("int32"), Result_t(Token_type::Int32, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint32"), Result_t(Token_type::Uint32, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("int64"), Result_t(Token_type::Int64, 5));
    ASSERT_EQ(lexer.tokenize<Token_type>("uint64"), Result_t(Token_type::Uint64, 6));

    ASSERT_EQ(lexer.tokenize<Token_type>("variable_name_1"), Result_t(Token_type::Identifier, 15));

    ASSERT_EQ(lexer.tokenize<Token_type>("1234"), Result_t(Token_type::Integer_literal, 4));
    ASSERT_EQ(lexer.tokenize<Token_type>("\"hello world\""), Result_t(Token_type::String_literal, 13));
    ASSERT_EQ(lexer.tokenize<Token_type>("'a'"), Result_t(Token_type::Character_literal, 3));
    ASSERT_EQ(lexer.tokenize<Token_type>("L\"wide string\""), Result_t(Token_type::Wide_string_literal, 14));
    ASSERT_EQ(lexer.tokenize<Token_type>("L'a'"), Result_t(Token_type::Wide_character_literal, 4));

    ASSERT_EQ(lexer.tokenize<Token_type>("123.45"), Result_t(Token_type::Fixed_point_literal, 6));
    ASSERT_EQ(lexer.tokenize<Token_type>("3.14159e+2"), Result_t(Token_type::Floating_point_literal, 10));

    ASSERT_EQ(lexer.tokenize<Token_type>("// a comment"), Result_t(Token_type::Single_line_comment, 12));
    ASSERT_EQ(lexer.tokenize<Token_type>("/* a comment */"), Result_t(Token_type::Multi_line_comment, 15));
}
