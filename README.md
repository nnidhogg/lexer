# **Lexer Library**

[![CI](https://github.com/nnidhogg/lexer/actions/workflows/ci.yml/badge.svg)](https://github.com/nnidhogg/lexer/actions/workflows/ci.yml)
[![codecov](https://codecov.io/github/nnidhogg/lexer/graph/badge.svg?token=2DTIFVQ4FN)](https://codecov.io/github/nnidhogg/lexer)

`lexer` is a **modern lexer library** that leverages **C++23** features. It provides a **builder-based API** that
enables flexible and customizable lexical analysis with combinator-based regular expressions. The library is inspired
by automata theory and uses **subset construction** to transform a Non-deterministic Finite Automaton (NFA) into an
equivalent Deterministic Finite Automaton (DFA), ensuring fast and reliable tokenization.

## **Features**

- **Highly Flexible Token Definition**  
  No predefined tokens — users can manually define tokens by specifying patterns and priorities, making the library
  adaptable to any language or custom syntax.

- **Modern C++23 Syntax**  
  Leverages C++23 features such as concise lambdas, enhanced template support, and ranges, improving both readability
  and maintainability.

- **Lightweight and Easy Integration**  
  Designed to be **linked into other projects** as a dependency, making it an excellent choice for parsers, compilers,
  and any project requiring lexical analysis.

- **Regex-like Combinators**  
  Provides combinator functions (`concat`, `choice`, `plus`, `kleene`, `optional`, `exact`, `at_least`, `range`, etc.)
  to create complex token patterns in an expressive and composable manner.

## **Usage Overview**

### **Defining Token Types**

Token types are defined as an `enum`:

```cpp
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
```

### **Registering Tokens with the Builder**

Tokens are registered using the `lexer::Builder` API:

```cpp
using namespace lexer;

Builder builder;

// Register keyword tokens
builder.add_token(text("boolean"), Token_type::Boolean, 1);
builder.add_token(text("char"), Token_type::Char, 1);

// Create and register identifier and literal tokens
const auto identifier{concat(any_of(Set::alpha() + '_'), kleene(any_of(Set::alphanum() + '_')))};
const auto integer_literal{plus(any_of(Set::digits()))};

builder.add_token(identifier, Token_type::Identifier, 4);
builder.add_token(integer_literal, Token_type::Integer_literal, 2);

// Build the lexer
const auto lexer{builder.build()};
```

- **Token Patterns**: Patterns can represent fixed strings (e.g., keywords) or complex regex-like expressions (e.g.
  identifiers, literals).
- **Priority**: Lower priority numbers are matched first, enabling conflict resolution for overlapping token patterns
  and ensuring the correct token is selected when multiple patterns match.

### **Tokenization**

`lexer` allows tokenizing input from various sources, including standard containers and iterators. You can pass a
container like `std::array` or provide a pair of iterators for flexible input handling.

```cpp
std::array<char, 5> input = {'1', '2', '3', '4', '\0'};
const auto [token, offset] = lexer.tokenize<Token_type>(input);

// token  -> Token_type::Integer_literal
// offset -> 4
```

Alternatively, you can tokenize a string using iterators:

```cpp
std::string input = "boolean";
const auto [token, offset] = lexer.tokenize<Token_type>(input.begin(), input.end());

// token  -> Token_type::Boolean
// offset -> 7
```

In both cases, the lexer correctly identifies the token and returns both the **token type** and the **length** of the
matched input (offset).

## **API Summary**

### **Combinator Functions**

- `text(...)`: Matches a sequence of characters.
- `any_of(...)`: Matches any character from a specified set.
- `concat(...)`: Concatenates multiple patterns in sequence.
- `choice(...)`: Chooses between multiple patterns.
- `plus(...)`: Matches one or more repetitions of a pattern.
- `kleene(...)`: Matches zero or more repetitions of a pattern.
- `optional(...)`: Matches zero or one occurrence of a pattern.
- `exact(count)`: Matches exactly `count` repetitions of a pattern.
- `at_least(min)`: Matches at least `min` repetitions of a pattern.
- `range(min, max)`: Matches between `min` and `max` repetitions of a pattern.

### **Builder Methods**

- `add_token(pattern, token_type, priority)`: Registers a token with a pattern, type, and priority.
- `build()`: Builds and returns the lexer.

## **Integration Instructions**

Since `lexer` is a **library**, it is designed to be used by other C++ projects. You can integrate it into your project
by:

### **Option 1: Add as a Submodule**

1. Add this repository as a submodule to your project:

```bash
git submodule add https://github.com/nnidhogg/lexer.git
```

2. Include the lexer library in your project’s build system (e.g., CMake).

### **Option 2: Directly Include Source Files**

1. Download or clone the repository.
2. Include the relevant source files in your project.
3. Ensure your project is compiled with **C++23** support.

## **Example CMake Integration**

Here’s a sample `CMakeLists.txt` for integrating the `lexer` library:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the lexer library
add_subdirectory(lexer)

# Link the lexer library to your target
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE lexer)
```

## **License**

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
