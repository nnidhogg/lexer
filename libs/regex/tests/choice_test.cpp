#include "lexer/regex/choice.hpp"

#include <gtest/gtest.h>

#include <filesystem>

#include "lexer/nfa/tools/graphviz.hpp"
#include "lexer/nfa/simulator.hpp"
#include "lexer/regex/text.hpp"

using namespace lexer::nfa;
using namespace lexer::nfa::tools;
using namespace lexer::regex;

class Choice_test : public testing::Test
{
protected:
    void write_dot(const auto& nfa, const std::string& name) const
    {
        Graphviz::to_file(nfa, debug_path_ / (name + ".dot"));
    }

private:
    std::filesystem::path debug_path_{std::string(SOURCE_DIR) + "/debug/"};
};

TEST_F(Choice_test, Single_character)
{
    using namespace testing;

    auto a{text('a')};
    auto b{text('b')};

    const auto regex{choice(a, b)};

    const Token token{1, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(nfa, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "b"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "c"), Result_t(std::nullopt, 0));
}

TEST_F(Choice_test, Multiple_characters)
{
    using namespace testing;

    auto a{text('a')};
    auto bc{text("bc")};
    auto def{text("def")};

    const auto regex{choice(a, bc, def)};

    const Token token{2, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(nfa, "a"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "bc"), Result_t(token, 2));
    EXPECT_EQ(Simulator::run(nfa, "def"), Result_t(token, 3));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "b"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "de"), Result_t(std::nullopt, 0));
}
