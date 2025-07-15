# MorseCodeTranslator

A modern C++17 class that translates alphanumeric text and common prosigns into International Morse code according to the ITU-R M.1677-1 specification.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Features

* Supports:

  * Letters A–Z
  * Digits 0–9
  * Standard punctuation marks
  * Prosigns: `AR`, `BT`, and `SK` as standalone words
* Formats Morse output with:

  * 1 space between dits/dahs
  * 3 spaces between letters
  * 7 spaces between words
* Throws `std::invalid_argument` for unsupported characters
* Provides:

  * Full translation via `getMessage()`
  * Incremental access via `getNext()`

## Usage

```cpp
#include "MorseCodeTranslator.hpp"

MorseCodeTranslator morse_message;
morse_message.setMessage(std::string("CQ AR DE K"));

// Get entire translation
std::string morse = morse_message.getMessage();
std::cout << morse << std::endl;

// Get word-by-word translation
std::string next;
while ((next = morse_message.getNext()) != "<EOM>") {
    std::cout << next << std::endl;
}
```

Note: `setMessage()` is overloaded for a `std::string` or `std::string_view`. You must explicitly construct one or the other:

```cpp
MorseCodeTranslator morse_message;
morse_message.setMessage(std::string("CQ AR DE K"));
```

or

```cpp
MorseCodeTranslator morse_message;
morse_message.setMessage(std::string_view("CQ AR DE K"));
```

## Prosigns

| Prosign | Morse Code    | Meaning             |
| ------- | ------------- | ------------------- |
| `AR`    | `. - . - .`   | End of message      |
| `BT`    | `- . . . -`   | Paragraph break     |
| `SK`    | `. . . - . -` | End of contact / SK |

Prosigns must be entered as standalone words (e.g., `"TEST AR"`, not `"TESTAR"`).

## Requirements

* C++17 compiler
* No external dependencies

## Build Instructions

This is a header-only class. You can include it directly in your project.

```sh
g++ -std=c++17 -o test test.cpp
./test
```

## Test Case Example

```cpp
#include "MorseCodeTranslator.hpp"
#include <cassert>

int main() {
    MorseCodeTranslator morse_message;
    morse_message.setMessage("AR SK");
    assert(morse_message.getNext() == ". - . - .");
    assert(morse_message.getNext() == ". . . - . -");
    assert(morse_message.getNext() == "<EOM>");
    return 0;
}
```

## License

This project is licensed under the [MIT License](LICENSE.md).

## Reference

* [ITU-R M.1677-1 Specification](https://www.itu.int/rec/R-REC-M.1677-1-200910-I)

---

### Note

Only punctuation defined in the standard is supported. All other symbols will trigger an exception.
