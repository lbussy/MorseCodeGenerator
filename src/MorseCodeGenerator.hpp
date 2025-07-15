/**
 * @file MorseCodeGenerator.hpp
 *
 * This project is is licensed under the MIT License. See LICENSE.MIT.md
 * for more information.
 *
 * Copyright (C) 2025 Lee C. Bussy (@LBussy). All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#ifndef MORSE_CODE_GENERATOR_HPP
#define MORSE_CODE_GENERATOR_HPP

#include <unordered_map>
#include <string>
#include <string_view>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <vector>

/**
 * @class MorseCodeGenerator
 * @brief Translates alphanumeric text and prosigns into Morse code.
 *
 * Supports letters (A–Z), digits (0–9), punctuation, and prosigns
 * defined in ITU-R M.1677-1, including AR, SK, and BT as standalone
 * words.
 *
 * Unsupported characters will throw an exception during translation.
 */
class MorseCodeGenerator
{
public:
    /**
     * @brief Default constructor.
     */
    MorseCodeGenerator() = default;

    /**
     * @brief Sets the message to be translated.
     *
     * @param msg The input text message to be converted to Morse code.
     */
    void setMessage(const std::string &msg)
    {
        message = msg;
        words = tokenizeMessage(message);
        wordIndex = 0;
    }

    /**
     * @brief Sets the message from a string_view.
     *
     * @param msg The input text message to be converted to Morse code.
     */
    void setMessage(std::string_view msg)
    {
        setMessage(std::string(msg));
    }

    /**
     * @brief Clears the stored message and resets internal state.
     */
    void clearMessage()
    {
        message.clear();
        words.clear();
        wordIndex = 0;
    }

    /**
     * @brief Returns the entire translated message as a Morse code string.
     *
     * Letters are separated by 3 spaces. Words are separated by 7 spaces.
     * Recognized prosigns like "AR", "BT", and "SK" are translated directly.
     *
     * @throws std::invalid_argument If an unsupported character is encountered.
     * @return Morse-encoded string with proper spacing.
     */
    std::string getMessage() const
    {
        std::string result;
        bool firstWord = true;

        for (const auto &word : tokenizeMessage(message))
        {
            if (!firstWord)
            {
                result += "       "; // 7 spaces between words
            }

            if (prosigns.count(word))
            {
                result += prosigns.at(word);
            }
            else
            {
                bool firstChar = true;
                for (char c : word)
                {
                    c = std::toupper(c);
                    auto it = morseTable.find(c);
                    if (it == morseTable.end())
                    {
                        throw std::invalid_argument("Unsupported character: " + std::string(1, c));
                    }
                    if (!firstChar)
                    {
                        result += "   "; // 3 spaces between letters
                    }
                    result += it->second;
                    firstChar = false;
                }
            }

            firstWord = false;
        }

        return result;
    }

    /**
     * @brief Returns the next word or prosign in Morse code.
     *
     * Internally tracks word position. Returns "<EOM>" when all words
     * have been translated.
     *
     * @throws std::invalid_argument If an unsupported character is encountered.
     * @return Morse-encoded word or prosign, or "<EOM>" at the end.
     */
    std::string getNext()
    {
        if (wordIndex >= words.size())
        {
            return "<EOM>";
        }

        const auto &word = words[wordIndex++];
        if (prosigns.count(word))
        {
            return prosigns.at(word);
        }

        std::string result;
        bool first = true;
        for (char c : word)
        {
            c = std::toupper(c);
            auto it = morseTable.find(c);
            if (it == morseTable.end())
            {
                throw std::invalid_argument("Unsupported character: " + std::string(1, c));
            }
            if (!first)
            {
                result += "   ";
            }
            result += it->second;
            first = false;
        }

        return result;
    }

private:
    std::string message;
    std::vector<std::string> words;
    size_t wordIndex = 0;

    std::vector<std::string> tokenizeMessage(const std::string &msg) const
    {
        std::vector<std::string> tokens;
        std::istringstream stream(msg);
        std::string word;
        while (stream >> word)
        {
            for (char &c : word)
            {
                c = std::toupper(c);
            }
            tokens.push_back(word);
        }
        return tokens;
    }

    const std::unordered_map<char, std::string> morseTable{
        {'A', ". -"}, {'B', "- . . ."}, {'C', "- . - ."}, {'D', "- . ."}, {'E', "."},
        {'F', ". . - ."}, {'G', "- - ."}, {'H', ". . . ."}, {'I', ". ."}, {'J', ". - - -"},
        {'K', "- . -"}, {'L', ". - . ."}, {'M', "- -"}, {'N', "- ."}, {'O', "- - -"},
        {'P', ". - - ."}, {'Q', "- - . -"}, {'R', ". - ."}, {'S', ". . ."}, {'T', "-"},
        {'U', ". . -"}, {'V', ". . . -"}, {'W', ". - -"}, {'X', "- . . -"}, {'Y', "- . - -"},
        {'Z', "- - . ."},
        {'0', "- - - - -"}, {'1', ". - - - -"}, {'2', ". . - - -"}, {'3', ". . . - -"},
        {'4', ". . . . -"}, {'5', ". . . . ."}, {'6', "- . . . ."}, {'7', "- - . . ."},
        {'8', "- - - . ."}, {'9', "- - - - ."},
        {'.', ". - . - . -"}, {',', "- - . . - -"}, {':', "- - - . . ."}, {'?', ". . - - . ."},
        {'/', "- . . - ."}, {'-', "- . . . . -"}, {'(', "- . - - . -"}, {')', "- . - - . -"},
        {'=', "- . . . -"}, {'+', ". - . - ."}, {'&', ". - . . ."}, {'\'', ". - - - - ."},
        {'!', "- . - . - -"}, {'_', ". . - - . -"}, {'"', ". - . . - ."},
        {'$', ". . . - . . -"}, {'@', ". - - . - ."},
    };

    const std::unordered_map<std::string, std::string> prosigns{
        {"AR", ". - . - ."},
        {"SK", ". . . - . -"},
        {"BT", "- . . . -"},
    };
};

#endif // MORSE_CODE_GENERATOR_HPP
