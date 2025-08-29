#include "lexer/regex/any_of.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>

#include "lexer/nfa/simulator.hpp"
#include "lexer/nfa/tools/graphviz.hpp"
#include "lexer/regex/set.hpp"

using namespace lexer;
using namespace lexer::nfa;
using namespace lexer::nfa::tools;
using namespace lexer::regex;

class Any_of_test : public testing::Test
{
protected:
    void write_dot(const auto& nfa, const std::string& name) const
    {
        Graphviz::to_file(nfa, debug_path_ / (name + ".dot"));
    }

private:
    std::filesystem::path debug_path_{std::string(SOURCE_DIR) + "/debug/"};
};

TEST_F(Any_of_test, Single_char)
{
    using namespace testing;

    const Set single_char{'a'};

    const auto regex{any_of(single_char)};

    const Token token{1, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(nfa, "a"), Result_t(token, 1));

    for (const auto set = Set::all() - single_char; auto symbol : set.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(std::nullopt, 0));
    }

    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(std::nullopt, 0));
}

TEST_F(Any_of_test, Multiple_chars)
{
    const Set multiple_chars{'a', 'b', 'c'};

    const auto regex{any_of(multiple_chars)};

    const auto builder{regex->to_nfa()};

    EXPECT_EQ(builder.init_state(), 0);

    EXPECT_EQ(builder.accept_states().size(), 1);
    EXPECT_TRUE(builder.accept_states().contains(1));

    EXPECT_EQ(builder.transitions().size(), 3);

    EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label('a')}));
    EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label('b')}));
    EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label('c')}));

    EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label('a')}).size(), 1);
    EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label('b')}).size(), 1);
    EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label('c')}).size(), 1);
}

TEST_F(Any_of_test, Simulate_multiple_char)
{
    using namespace testing;

    const Set multiple_chars{'a', 'b', 'c'};

    const auto regex{any_of(multiple_chars)};

    const Token token{2, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    for (const auto symbol : multiple_chars.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(token, 1));
    }

    for (const auto set = Set::all() - multiple_chars; const auto symbol : set.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(std::nullopt, 0));
    }

    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(std::nullopt, 0));
}

TEST_F(Any_of_test, Alpha_chars)
{
    const auto alpha_chars{Set::alpha()};

    const auto regex{any_of(alpha_chars)};

    const auto builder{regex->to_nfa()};

    EXPECT_EQ(builder.init_state(), 0);

    EXPECT_EQ(builder.accept_states().size(), 1);
    EXPECT_TRUE(builder.accept_states().contains(1));

    EXPECT_EQ(builder.transitions().size(), alpha_chars.symbols().size());

    std::ranges::for_each(alpha_chars.symbols(), [&builder](const auto symbol) {
        EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label{symbol}}));
        EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label{symbol}}).size(), 1);
    });
}

TEST_F(Any_of_test, Simulate_alpha_chars)
{
    using namespace testing;

    const auto alpha_chars{Set::alpha()};

    const auto regex{any_of(alpha_chars)};

    const Token token{3, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    for (const auto symbol : alpha_chars.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(token, 1));
    }

    for (const auto set = Set::all() - alpha_chars; const auto symbol : set.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(std::nullopt, 0));
    }

    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(std::nullopt, 0));
}

TEST_F(Any_of_test, Digit_chars)
{
    const auto digit_chars{Set::digits()};

    const auto regex{any_of(digit_chars)};

    const auto builder{regex->to_nfa()};

    EXPECT_EQ(builder.init_state(), 0);

    EXPECT_EQ(builder.accept_states().size(), 1);
    EXPECT_TRUE(builder.accept_states().contains(1));

    EXPECT_EQ(builder.transitions().size(), digit_chars.symbols().size());

    std::ranges::for_each(digit_chars.symbols(), [&builder](const auto symbol) {
        EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label{symbol}}));
        EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label{symbol}}).size(), 1);
    });
}

TEST_F(Any_of_test, Simulate_digit_chars)
{
    using namespace testing;

    const auto digit_chars{Set::digits()};

    const auto regex{any_of(digit_chars)};

    const Token token{4, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    for (const auto symbol : digit_chars.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(token, 1));
    }

    for (const auto set = Set::all() - digit_chars; const auto symbol : set.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(std::nullopt, 0));
    }

    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(std::nullopt, 0));
}

TEST_F(Any_of_test, Alphanum_chars)
{
    const auto alphanum_chars{Set::alphanum()};

    const auto regex{any_of(alphanum_chars)};

    const auto builder{regex->to_nfa()};

    EXPECT_EQ(builder.init_state(), 0);

    EXPECT_EQ(builder.accept_states().size(), 1);
    EXPECT_TRUE(builder.accept_states().contains(1));

    EXPECT_EQ(builder.transitions().size(), alphanum_chars.symbols().size());

    std::ranges::for_each(alphanum_chars.symbols(), [&builder](const auto symbol) {
        EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label{symbol}}));
        EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label{symbol}}).size(), 1);
    });
}

TEST_F(Any_of_test, Simulate_alphanum_chars)
{
    using namespace testing;

    const auto alphanum_chars{Set::alphanum()};

    const auto regex{any_of(alphanum_chars)};

    const Token token{5, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    for (const auto symbol : alphanum_chars.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(token, 1));
    }

    for (const auto set = Set::all() - alphanum_chars; const auto symbol : set.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(std::nullopt, 0));
    }

    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(std::nullopt, 0));
}

TEST_F(Any_of_test, Printable_chars)
{
    const auto printable_chars{Set::printable()};

    const auto regex{any_of(printable_chars)};

    const auto builder{regex->to_nfa()};

    EXPECT_EQ(builder.init_state(), 0);

    EXPECT_EQ(builder.accept_states().size(), 1);
    EXPECT_TRUE(builder.accept_states().contains(1));

    EXPECT_EQ(builder.transitions().size(), printable_chars.symbols().size());

    std::ranges::for_each(printable_chars.symbols(), [&builder](const auto symbol) {
        EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label{symbol}}));
        EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label{symbol}}).size(), 1);
    });
}

TEST_F(Any_of_test, Simulate_printable_chars)
{
    using namespace testing;

    const auto printable_chars{Set::printable()};

    const auto regex{any_of(printable_chars)};

    const Token token{6, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    for (const auto symbol : printable_chars.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(token, 1));
    }

    for (const auto set = Set::all() - printable_chars; const auto symbol : set.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(std::nullopt, 0));
    }

    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(token, 1));

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
}

TEST_F(Any_of_test, All_chars)
{
    const auto all_chars{Set::all()};

    const auto regex{any_of(all_chars)};

    const auto builder{regex->to_nfa()};

    EXPECT_EQ(builder.init_state(), 0);

    EXPECT_EQ(builder.accept_states().size(), 1);
    EXPECT_TRUE(builder.accept_states().contains(1));

    EXPECT_EQ(builder.transitions().size(), all_chars.symbols().size());

    std::ranges::for_each(all_chars.symbols(), [&builder](const auto symbol) {
        EXPECT_TRUE(builder.transitions().contains({builder.init_state(), nfa::Label{symbol}}));
        EXPECT_EQ(builder.transitions().at({builder.init_state(), nfa::Label{symbol}}).size(), 1);
    });
}

TEST_F(Any_of_test, Simulate_all_chars)
{
    using namespace testing;

    const auto all_chars{Set::all()};

    const auto regex{any_of(all_chars)};

    const Token token{7, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    for (const auto symbol : all_chars.symbols())
    {
        EXPECT_EQ(Simulator::run(nfa, std::string{symbol}), Result_t(token, 1));
    }

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "ba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "cba"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "bac"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "123"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "*[=]"), Result_t(token, 1));
    EXPECT_EQ(Simulator::run(nfa, "a.b.c"), Result_t(token, 1));
}

TEST_F(Any_of_test, Empty_set)
{
    const Set empty{};

    const auto regex{any_of(empty)};

    const Token token{7, 1};

    const auto nfa{regex->to_nfa().set_accept_token(token).build()};

    using Result_t = Simulator::Result_t;

    EXPECT_EQ(Simulator::run(nfa, ""), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "a"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "ab"), Result_t(std::nullopt, 0));
    EXPECT_EQ(Simulator::run(nfa, "abc"), Result_t(std::nullopt, 0));
}
