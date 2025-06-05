/**
 * @file main.cpp
 * @brief Demonstrates and tests the MorseCodeTranslator class.
 *
 * This program sets various messages, translates them to Morse code, outputs the
 * result word-by-word, and verifies prosign handling and invalid character rejection.
 */

#include "MorseCodeTranslator.hpp"
#include <iostream>
#include <cassert>

/**
 * @brief Main entry point for Morse code translator test.
 *
 * This function exercises:
 * - Full message translation
 * - Word-by-word iteration
 * - Assertion-based prosign testing
 * - Exception handling for unsupported characters
 *
 * @return int 0 if successful, non-zero on unexpected error.
 */
int main() {
    try {
        MorseCodeTranslator tx;

        std::cout << "[Test] Setting message: CQ AR DE K" << std::endl;
        tx.setMessage("CQ AR DE K");

        std::cout << "[Test] Full Morse message:" << std::endl;
        std::cout << tx.getMessage() << std::endl;

        std::cout << "[Test] Word-by-word output:" << std::endl;
        std::string part;
        while ((part = tx.getNext()) != "<EOM>") {
            std::cout << part << std::endl;
        }

        std::cout << "[Test] Reset and check prosigns AR SK" << std::endl;
        tx.setMessage("AR SK");
        assert(tx.getNext() == ". - . - .");  // AR
        assert(tx.getNext() == ". . . - . -"); // SK
        assert(tx.getNext() == "<EOM>");

        std::cout << "[Test Passed] Prosign test successful." << std::endl;

        std::cout << "[Test] Invalid character test (expected to throw):" << std::endl;
        tx.setMessage("HELLO @ WORLD");  // Valid input
        std::cout << tx.getMessage() << std::endl;

        tx.setMessage("HELLO ~ WORLD");  // ~ is unsupported and should throw
        std::cout << tx.getMessage() << std::endl;

    } catch (const std::invalid_argument& e) {
        std::cerr << "[Caught Exception] " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[Error] Unexpected exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
