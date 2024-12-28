#include "lexer/dfa/dfa.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "lexer/dfa/builder.hpp"
#include "lexer/dfa/simulator.hpp"
#include "lexer/dfa/tools/graphviz.hpp"

using namespace lexer;
using namespace lexer::dfa;
using namespace lexer::dfa::tools;

class Dfa_test : public testing::Test
{
protected:
    void write_dot(const auto& dfa, const std::string& file_name) const
    {
        Graphviz::to_file(dfa, debug_path_ / (file_name + ".dot"));
    }

private:
    std::filesystem::path debug_path_{std::string(SOURCE_DIR) + "/debug/"};
};

TEST_F(Dfa_test, Test_empty)
{
    const dfa::Builder dfa;

    const auto result{dfa.build()};

    constexpr std::vector<char> input;

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, input), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Any_of)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('a'), q0);

    dfa.add_transition(q0, dfa::Label('b'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "b"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(result, "ab"), Result_t(token, 2));
    EXPECT_EQ(Simulator::run(result, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(result, "aab"), Result_t(token, 3));
    EXPECT_EQ(Simulator::run(result, "baa"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(result, "aaab"), Result_t(token, 4));
    EXPECT_EQ(Simulator::run(result, "baaa"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(result, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "aa"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "aaa"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "aaaa"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Single_character)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(result, "aa"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "b"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Optional_character)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token_empty{1};
    const Token token_a{2};

    dfa.add_accept_state(q0, token_empty);
    dfa.add_accept_state(q1, token_a);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, ""), Result_t(token_empty, 0));
    EXPECT_EQ(Simulator::run(result, "a"), Result_t(token_a, 1));
    EXPECT_EQ(Simulator::run(result, "aa"), Result_t(token_a, 1));

    EXPECT_EQ(Simulator::run(result, "b"), Result_t(token_empty, 0));
    EXPECT_EQ(Simulator::run(result, "ba"), Result_t(token_empty, 0));
}

TEST_F(Dfa_test, Sequence_ab)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};
    const auto q2{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q2, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('b'), q2);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "ab"), Result_t(token, 2));
    EXPECT_EQ(Simulator::run(result, "abc"), Result_t(token, 2));

    EXPECT_EQ(Simulator::run(result, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "b"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Kleene_star_a)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};

    const Token token{1};

    dfa.add_accept_state(q0, token);

    dfa.add_transition(q0, dfa::Label('a'), q0);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, ""), Result_t(token, 0));
    EXPECT_EQ(Simulator::run(result, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(result, "aa"), Result_t(token, 2));
    EXPECT_EQ(Simulator::run(result, "aaa"), Result_t(token, 3));
    EXPECT_EQ(Simulator::run(result, "aaab"), Result_t(token, 3));

    EXPECT_EQ(Simulator::run(result, "b"), Result_t(token, 0));
    EXPECT_EQ(Simulator::run(result, "ba"), Result_t(token, 0));
    EXPECT_EQ(Simulator::run(result, "baa"), Result_t(token, 0));
    EXPECT_EQ(Simulator::run(result, "baaa"), Result_t(token, 0));
}

TEST_F(Dfa_test, Branch_ab)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};
    const auto q2{dfa.next_state()};

    const Token token_a{1};
    const Token token_b{2};

    dfa.add_accept_state(q1, token_a);
    dfa.add_accept_state(q2, token_b);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q0, dfa::Label('b'), q2);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "a"), Result_t(token_a, 1));
    EXPECT_EQ(Simulator::run(result, "b"), Result_t(token_b, 1));
    EXPECT_EQ(Simulator::run(result, "ab"), Result_t(token_a, 1));
    EXPECT_EQ(Simulator::run(result, "aa"), Result_t(token_a, 1));

    EXPECT_EQ(Simulator::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "c"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "ca"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "cb"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Repeat_abc)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};
    const auto q2{dfa.next_state()};
    const auto q3{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q3, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('b'), q2);

    dfa.add_transition(q2, dfa::Label('c'), q3);

    dfa.add_transition(q3, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "abc"), Result_t(token, 3));
    EXPECT_EQ(Simulator::run(result, "abca"), Result_t(token, 3));
    EXPECT_EQ(Simulator::run(result, "abcabc"), Result_t(token, 6));
    EXPECT_EQ(Simulator::run(result, "abcabcabc"), Result_t(token, 9));

    EXPECT_EQ(Simulator::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "ab"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Contain_ab)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};
    const auto q2{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q2, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('b'), q2);

    dfa.add_transition(q0, dfa::Label('x'), q0);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "ab"), Result_t(token, 2));
    EXPECT_EQ(Simulator::run(result, "xxab"), Result_t(token, 4));

    EXPECT_EQ(Simulator::run(result, "ax"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Numeric_branch)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};
    const auto q2{dfa.next_state()};
    const auto q3{dfa.next_state()};
    const auto q4{dfa.next_state()};
    const auto q5{dfa.next_state()};

    const Token token_123{1};
    const Token token_45{1};

    dfa.add_accept_state(q3, token_123);
    dfa.add_accept_state(q5, token_45);

    dfa.add_transition(q0, dfa::Label('1'), q1);
    dfa.add_transition(q1, dfa::Label('2'), q2);
    dfa.add_transition(q2, dfa::Label('3'), q3);

    dfa.add_transition(q0, dfa::Label('4'), q4);
    dfa.add_transition(q4, dfa::Label('5'), q5);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "45"), Result_t(token_45, 2));
    EXPECT_EQ(Simulator::run(result, "123"), Result_t(token_123, 3));
    EXPECT_EQ(Simulator::run(result, "1234"), Result_t(token_123, 3));

    EXPECT_EQ(Simulator::run(result, "12"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "124"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "467"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Loop_plus_a)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(result, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(result, "aa"), Result_t(token, 2));
    EXPECT_EQ(Simulator::run(result, "aaa"), Result_t(token, 3));
    EXPECT_EQ(Simulator::run(result, "aaaa"), Result_t(token, 4));

    EXPECT_EQ(Simulator::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(result, "b"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Graphviz_to_dot)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);
    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    const std::string dot_output = Graphviz::to_dot(result);

    const std::string expected_output =
            "digraph DFA {\n"
            "    rankdir=LR;\n"
            "    ratio=1.0;\n"
            "    node [shape = circle];\n"
            "    1 [shape = doublecircle, label=\"1 (1)\"];\n"
            "    __start__ [shape = none, label=\"\"];\n"
            "    __start__ -> 0;\n"
            "    0 -> 1 [label = \"a\"];\n"
            "}\n";

    EXPECT_EQ(dot_output, expected_output);
}

TEST_F(Dfa_test, Graphviz_to_file)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);
    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    const std::filesystem::path file_path{"./dfa_test_output.dot"};
    Graphviz::to_file(result, file_path);

    std::ifstream file(file_path);
    ASSERT_TRUE(file.is_open());

    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string expected_output{
            "digraph DFA {\n"
            "    rankdir=LR;\n"
            "    ratio=1.0;\n"
            "    node [shape = circle];\n"
            "    1 [shape = doublecircle, label=\"1 (1)\"];\n"
            "    __start__ [shape = none, label=\"\"];\n"
            "    __start__ -> 0;\n"
            "    0 -> 1 [label = \"a\"];\n"
            "}\n"};

    EXPECT_EQ(buffer.str(), expected_output);
}

TEST_F(Dfa_test, Graphviz_to_file_exceptions)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);
    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    // Test invalid file path
    EXPECT_THROW(Graphviz::to_file(result, ""), std::runtime_error);

    // Test valid file path
    const std::filesystem::path file_path{"./dfa_test_output.dot"};
    EXPECT_NO_THROW(Graphviz::to_file(result, file_path));

    std::ifstream file(file_path);
    ASSERT_TRUE(file.is_open());

    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string expected_output{
            "digraph DFA {\n"
            "    rankdir=LR;\n"
            "    ratio=1.0;\n"
            "    node [shape = circle];\n"
            "    1 [shape = doublecircle, label=\"1 (1)\"];\n"
            "    __start__ [shape = none, label=\"\"];\n"
            "    __start__ -> 0;\n"
            "    0 -> 1 [label = \"a\"];\n"
            "}\n"};

    EXPECT_EQ(buffer.str(), expected_output);
}

TEST_F(Dfa_test, Graphviz_to_dot_special_characters)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('"'), q1);
    dfa.add_transition(q0, dfa::Label('\\'), q1);
    dfa.add_transition(q0, dfa::Label('\n'), q1);
    dfa.add_transition(q0, dfa::Label('\t'), q1);

    const auto result{dfa.build()};

    const auto dot_output{Graphviz::to_dot(result)};

    // Check that all expected transitions are present in the output
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\t\"]"), std::string::npos);
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\n\"]"), std::string::npos);
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\\\\"]"), std::string::npos);
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\\"\"]"), std::string::npos);

    // Check other structural elements of the dot output
    EXPECT_NE(dot_output.find("rankdir=LR"), std::string::npos);
    EXPECT_NE(dot_output.find("node [shape = circle]"), std::string::npos);
    EXPECT_NE(dot_output.find("1 [shape = doublecircle, label=\"1 (1)\"]"), std::string::npos);
}

TEST_F(Dfa_test, Graphviz_to_dot_non_printable_characters)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.next_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    // Add transitions with non-printable characters
    dfa.add_transition(q0, dfa::Label(static_cast<char>(0x01)), q1); // SOH
    dfa.add_transition(q0, dfa::Label(static_cast<char>(0x7F)), q1); // DEL
    dfa.add_transition(q0, dfa::Label(static_cast<char>(0xFF)), q1); // Extended ASCII

    const auto result{dfa.build()};

    const auto dot_output{Graphviz::to_dot(result)};

    // Check that all expected transitions are present in the output
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\x01\"]"), std::string::npos);
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\x7F\"]"), std::string::npos);
    EXPECT_NE(dot_output.find("0 -> 1 [label = \"\\xFF\"]"), std::string::npos);

    // Check other structural elements of the dot output
    EXPECT_NE(dot_output.find("rankdir=LR"), std::string::npos);
    EXPECT_NE(dot_output.find("node [shape = circle]"), std::string::npos);
    EXPECT_NE(dot_output.find("1 [shape = doublecircle, label=\"1 (1)\"]"), std::string::npos);
}
