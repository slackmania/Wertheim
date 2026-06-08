#ifndef WERTHEIM_CIPHER_HPP
#define WERTHEIM_CIPHER_HPP

#include <string>
#include <array>

namespace wertheim {

// ──────────────────────────────────────────────────────────────
// WERTHEIM Cipher - Complete C++ Port of the Free Pascal original
//
// Dual-layer text encryption:
//   Layer 1: RC4 stream cipher keyed with SHA1 hash of the password
//            (via OpenSSL EVP)
//   Layer 2: Custom substitution cipher ("hinta") using 65×65
//            permutation tables derived from the password
//
// The original Pascal application (Lazarus / Free Pascal) was
// a GUI text editor that encrypts
// .wtx files. This C++ library replicates the exact cipher.
//
// Usage:
//   std::string encrypted = wertheim::hintabe(plaintext, password);
//   std::string decrypted = wertheim::hintaki(encrypted, password);
//
// Dependencies:
//   - OpenSSL (libcrypto) for RC4 + SHA1
//   - C++17 or later
// ──────────────────────────────────────────────────────────────

// ── Public API (matches hinta.pas) ────────────────────────────

// Encrypt plaintext with password (hintabe)
std::string hintabe(const std::string& mit, const std::string& jelszo);

// Decrypt ciphertext with password (hintaki)
std::string hintaki(const std::string& mit, const std::string& jelszo);

// ── Internal functions (exposed for testing) ──────────────────

// Custom Base64 (character set: 0-9 A-Z a-z +/)
// This is NOT standard Base64! The alphabet order is different.
std::string encode64(const std::string& s);
std::string decode64(const std::string& s);

// Core substitution encode/decode (single pass, needs precomputed hintar state)
std::string bekod(const std::string& mit, const std::string hintar[50], int pwdhossz);
std::string kikod(const std::string& mit, const std::string hintar[50], int pwdhossz);

// Full encode/decode (hinta + RC4 + Base64) with explicit state
std::string bekodol(const std::string& mit, const std::string& jelszo,
                    int kod[50], std::string hintar[50], int& pwdhossz, int& pwdcode);
std::string kikodol(const std::string& mit, const std::string& jelszo,
                    int kod[50], std::string hintar[50], int& pwdhossz, int& pwdcode);

// Password-to-key derivation
void pw_to_kod(const std::string& mit, int kod[50], int& pwdhossz, int& pwdcode);
void genhinta(const std::string& pwd, int kod[50], std::string hintar[50], int& pwdhossz, int& pwdcode);

// RC4+SHA1 layer (OpenSSL-based)
std::string besecure(const std::string& mit, const std::string& pw);
std::string kisecure(const std::string& mit, const std::string& pw);

// ── Constants ─────────────────────────────────────────────────

extern const std::string CODES64;   // Base64 alphabet
extern const std::string ABC0;      // Hinta substitution alphabet
extern const int MAX_PWD_LEN;       // Maximum password length (50)

} // namespace wertheim

#endif // WERTHEIM_CIPHER_HPP
