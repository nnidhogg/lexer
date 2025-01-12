# **Lexer Library**

`lexer` is a **modern lexer library** implemented with C++23 features and inspired by automata theory. It is designed
for seamless integration into other libraries or applications, providing flexible and customizable lexical analysis.
Unlike traditional lexer generators, `lexer` offers a **builder-based API** that allows developers to manually define
tokens using combinator-based regular expressions, making it highly adaptable to various languages and custom syntaxes.

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

The `tokenize` method supports two types of inputs, container-based tokenization and iterator-based tokenization:

#### Container-Based Tokenization

Example using container-based tokenization with `std::array`:

```cpp
std::array<char, 5> input = {'1', '2', '3', '4', '\0'};
const auto [token, offset] = lexer.tokenize<Token_type>(input);

// token  -> Token_type::Integer_literal
// offset -> 4
```

#### Iterator-Based Tokenization

Example using iterator-based tokenization:

```cpp
std::string input = "boolean";
const auto [token, offset] = lexer.tokenize<Token_type>(input.begin(), input.end());

// token  -> Token_type::Boolean
// offset -> 7
```

The lexer correctly identifies the token and returns both the **token type** and the **length** of the matched input
(offset).

## **API Summary**

### **Combinator Functions**

- `concat(...)`: Concatenates multiple patterns in sequence.
- `choice(...)`: Chooses between multiple patterns.
- `plus(...)`: Matches one or more repetitions of a pattern.
- `kleene(...)`: Matches zero or more repetitions of a pattern.
- `optional(...)`: Matches zero or one occurrence of a pattern.
- `exact(count)`: Matches exactly `count` repetitions of a pattern.
- `at_least(min)`: Matches at least `min` repetitions of a pattern.
- `range(min, max)`: Matches between `min` and `max` repetitions of a pattern.
- `any_of(...)`: Matches any character from a specified set.
- `optional(...)`: Matches zero or one occurrence of a pattern.
- `text(...)`: Matches a sequence of characters.

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
   git submodule update --init --recursive
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
