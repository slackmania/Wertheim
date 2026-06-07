#include "wertheim.hpp"
#include <iostream>
#include <cassert>

// ──────────────────────────────────────────────────────────────
// WERTHEIM Cipher - Example / Test Program
// ──────────────────────────────────────────────────────────────

static void test_basic_roundtrip() {
    std::cout << "=== Test: Basic round-trip ===" << std::endl;

    const std::string plaintext  = "Hello World! This is a secret message. 12345.";
    const std::string password   = "mySecretPassword";

    std::string encrypted = wertheim::hintabe(plaintext, password);
    std::string decrypted = wertheim::hintaki(encrypted, password);

    std::cout << "Plaintext:  " << plaintext  << std::endl;
    std::cout << "Password:   " << password   << std::endl;
    std::cout << "Encrypted:  " << encrypted  << std::endl;
    std::cout << "Decrypted:  " << decrypted  << std::endl;

    if (plaintext == decrypted) {
        std::cout << "[PASS] Round-trip successful" << std::endl;
    } else {
        std::cout << "[FAIL] Round-trip mismatch!" << std::endl;
    }
}

static void test_base64() {
    std::cout << "\n=== Test: Custom Base64 ===" << std::endl;

    const std::string original = "Hello World!";
    std::string encoded = wertheim::encode64(original);
    std::string decoded = wertheim::decode64(encoded);

    std::cout << "Original: " << original << std::endl;
    std::cout << "Encoded:  " << encoded  << std::endl;
    std::cout << "Decoded:  " << decoded  << std::endl;

    assert(original == decoded);
    std::cout << "[PASS] Base64 round-trip OK" << std::endl;
}

static void test_hungarian_text() {
    std::cout << "\n=== Test: Hungarian text (UTF-8) ===" << std::endl;

    const std::string plaintext  = "Árvíztűrő tükörfúrógép Őrült! QWERTZ";
    const std::string password   = "prutty";

    std::string encrypted = wertheim::hintabe(plaintext, password);
    std::string decrypted = wertheim::hintaki(encrypted, password);

    std::cout << "Plaintext:  " << plaintext  << std::endl;
    std::cout << "Password:   " << password   << std::endl;
    std::cout << "Encrypted:  " << encrypted  << std::endl;
    std::cout << "Decrypted:  " << decrypted  << std::endl;

    if (plaintext == decrypted) {
        std::cout << "[PASS] Hungarian text round-trip OK" << std::endl;
    } else {
        std::cout << "[FAIL] Round-trip mismatch!" << std::endl;
    }
}

static void test_empty_password() {
    std::cout << "\n=== Test: Empty password ===" << std::endl;

    const std::string plaintext  = "Test with empty password";

    try {
        std::string encrypted = wertheim::hintabe(plaintext, "");
        std::string decrypted = wertheim::hintaki(encrypted, "");
        std::cout << "Encrypted: " << encrypted << std::endl;
        std::cout << "Decrypted: " << decrypted << std::endl;
        if (plaintext == decrypted) {
            std::cout << "[PASS] Empty password round-trip OK" << std::endl;
        } else {
            std::cout << "[FAIL] Round-trip mismatch!" << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "[INFO] Exception with empty password: " << e.what() << std::endl;
    }
}

static void test_wrong_password() {
    std::cout << "\n=== Test: Wrong password (should produce garbage) ===" << std::endl;

    const std::string plaintext  = "This is secret";
    const std::string password1  = "correct password";
    const std::string password2  = "wrong password";

    std::string encrypted = wertheim::hintabe(plaintext, password1);
    std::string decrypted = wertheim::hintaki(encrypted, password2);

    std::cout << "Original:     " << plaintext  << std::endl;
    std::cout << "Decrypted(w): " << decrypted  << std::endl;

    if (plaintext != decrypted) {
        std::cout << "[PASS] Wrong password produces different output (as expected)" << std::endl;
    } else {
        std::cout << "[WARN] Wrong password still produced correct text (unlikely)" << std::endl;
    }
}

int main() {
    std::cout << "WERTHEIM Cipher - C++ Port Test Suite" << std::endl;
    std::cout << "=====================================" << std::endl;

    test_base64();
    test_basic_roundtrip();
    test_hungarian_text();
    test_empty_password();
    test_wrong_password();

    std::cout << "\n=== Demo: Encrypting/Decrypting from stdin ===" << std::endl;
    std::cout << "Enter text to encrypt: ";
    std::string input;
    std::getline(std::cin, input);

    std::cout << "Enter password: ";
    std::string pwd;
    std::getline(std::cin, pwd);

    std::string enc = wertheim::hintabe(input, pwd);
    std::cout << "Encrypted: " << enc << std::endl;

    std::string dec = wertheim::hintaki(enc, pwd);
    std::cout << "Decrypted: " << dec << std::endl;

    return 0;
}
