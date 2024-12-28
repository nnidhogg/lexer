#include "lexer/dfa/dfa.hpp"

#include <gtest/gtest.h>

#include <filesystem>

#include "lexer/dfa/builder.hpp"
#include "lexer/dfa/tools/graphviz.hpp"
#include "lexer/dfa/tools/simulation.hpp"

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

TEST_F(Dfa_test, Any_of)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('a'), q0);

    dfa.add_transition(q0, dfa::Label('b'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "b"), Result_t(token, 1));
    EXPECT_EQ(Simulation::run(result, "ab"), Result_t(token, 2));
    EXPECT_EQ(Simulation::run(result, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulation::run(result, "aab"), Result_t(token, 3));
    EXPECT_EQ(Simulation::run(result, "baa"), Result_t(token, 1));
    EXPECT_EQ(Simulation::run(result, "aaab"), Result_t(token, 4));
    EXPECT_EQ(Simulation::run(result, "baaa"), Result_t(token, 1));

    EXPECT_EQ(Simulation::run(result, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "aa"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "aaa"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "aaaa"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Single_character)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulation::run(result, "aa"), Result_t(token, 1));

    EXPECT_EQ(Simulation::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "b"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Optional_character)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};

    const Token token_empty{1};
    const Token token_a{2};

    dfa.add_accept_state(q0, token_empty);
    dfa.add_accept_state(q1, token_a);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, ""), Result_t(token_empty, 0));
    EXPECT_EQ(Simulation::run(result, "a"), Result_t(token_a, 1));
    EXPECT_EQ(Simulation::run(result, "aa"), Result_t(token_a, 1));

    EXPECT_EQ(Simulation::run(result, "b"), Result_t(token_empty, 0));
    EXPECT_EQ(Simulation::run(result, "ba"), Result_t(token_empty, 0));
}

TEST_F(Dfa_test, Sequence_ab)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};
    const auto q2{dfa.step_state()};

    const Token token{1};

    dfa.add_accept_state(q2, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('b'), q2);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "ab"), Result_t(token, 2));
    EXPECT_EQ(Simulation::run(result, "abc"), Result_t(token, 2));

    EXPECT_EQ(Simulation::run(result, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "b"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Kleene_star_a)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};

    const Token token{1};

    dfa.add_accept_state(q0, token);

    dfa.add_transition(q0, dfa::Label('a'), q0);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, ""), Result_t(token, 0));
    EXPECT_EQ(Simulation::run(result, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulation::run(result, "aa"), Result_t(token, 2));
    EXPECT_EQ(Simulation::run(result, "aaa"), Result_t(token, 3));
    EXPECT_EQ(Simulation::run(result, "aaab"), Result_t(token, 3));

    EXPECT_EQ(Simulation::run(result, "b"), Result_t(token, 0));
    EXPECT_EQ(Simulation::run(result, "ba"), Result_t(token, 0));
    EXPECT_EQ(Simulation::run(result, "baa"), Result_t(token, 0));
    EXPECT_EQ(Simulation::run(result, "baaa"), Result_t(token, 0));
}

TEST_F(Dfa_test, Branch_ab)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};
    const auto q2{dfa.step_state()};

    const Token token_a{1};
    const Token token_b{2};

    dfa.add_accept_state(q1, token_a);
    dfa.add_accept_state(q2, token_b);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q0, dfa::Label('b'), q2);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "a"), Result_t(token_a, 1));
    EXPECT_EQ(Simulation::run(result, "b"), Result_t(token_b, 1));
    EXPECT_EQ(Simulation::run(result, "ab"), Result_t(token_a, 1));
    EXPECT_EQ(Simulation::run(result, "aa"), Result_t(token_a, 1));

    EXPECT_EQ(Simulation::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "c"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "ca"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "cb"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Repeat_abc)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};
    const auto q2{dfa.step_state()};
    const auto q3{dfa.step_state()};

    const Token token{1};

    dfa.add_accept_state(q3, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('b'), q2);

    dfa.add_transition(q2, dfa::Label('c'), q3);

    dfa.add_transition(q3, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "abc"), Result_t(token, 3));
    EXPECT_EQ(Simulation::run(result, "abca"), Result_t(token, 3));
    EXPECT_EQ(Simulation::run(result, "abcabc"), Result_t(token, 6));
    EXPECT_EQ(Simulation::run(result, "abcabcabc"), Result_t(token, 9));

    EXPECT_EQ(Simulation::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "ab"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Contain_ab)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};
    const auto q2{dfa.step_state()};

    const Token token{1};

    dfa.add_accept_state(q2, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('b'), q2);

    dfa.add_transition(q0, dfa::Label('x'), q0);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "ab"), Result_t(token, 2));
    EXPECT_EQ(Simulation::run(result, "xxab"), Result_t(token, 4));

    EXPECT_EQ(Simulation::run(result, "ax"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Numeric_branch)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};
    const auto q2{dfa.step_state()};
    const auto q3{dfa.step_state()};
    const auto q4{dfa.step_state()};
    const auto q5{dfa.step_state()};

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

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "45"), Result_t(token_45, 2));
    EXPECT_EQ(Simulation::run(result, "123"), Result_t(token_123, 3));
    EXPECT_EQ(Simulation::run(result, "1234"), Result_t(token_123, 3));

    EXPECT_EQ(Simulation::run(result, "12"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "124"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "467"), Result_t(std::nullopt, 0));
}

TEST_F(Dfa_test, Loop_plus_a)
{
    dfa::Builder dfa;

    const auto q0{dfa.init_state()};
    const auto q1{dfa.step_state()};

    const Token token{1};

    dfa.add_accept_state(q1, token);

    dfa.add_transition(q0, dfa::Label('a'), q1);

    dfa.add_transition(q1, dfa::Label('a'), q1);

    const auto result{dfa.build()};

    using Result_t = Simulation::Result_t;

    EXPECT_EQ(Simulation::run(result, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulation::run(result, "aa"), Result_t(token, 2));
    EXPECT_EQ(Simulation::run(result, "aaa"), Result_t(token, 3));
    EXPECT_EQ(Simulation::run(result, "aaaa"), Result_t(token, 4));

    EXPECT_EQ(Simulation::run(result, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulation::run(result, "b"), Result_t(std::nullopt, 0));
}
