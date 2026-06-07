// Auto-generated from hinta.pas - WERTHEIM substitution tables (65x65)
// This file is part of the WERTHEIM cipher implementation
#ifndef WERTHEIM_TARR_DATA_HPP
#define WERTHEIM_TARR_DATA_HPP

#include <string>
#include <array>

namespace wertheim {

// The 65x65 substitution table from the original Pascal code.
// tarr[row][col] where both row and col range 0..64.
// Each entry is a 64-character permutation string used in the hinta cipher.
// WARNING: The original Pascal code accesses [kod[t]][ord(pwd[t])] where
// ord(pwd[t]) can exceed 64 for base64 chars (e.g., 'A'=65, 'z'=122).
// The C++ port allocates 65x256 to avoid out-of-bounds access for ASCII 0-127.
// Uninitialized entries (col > 64) remain empty strings.
// For full binary compatibility with the original Pascal EXE, notes:
//   The Pascal array was string[0..64,0..64]. Accessing [row][col] where
//   col > 64 reads adjacent memory. This C++ port defaults extra cols to "".

constexpr int TARR_ROWS = 65;
constexpr int TARR_COLS = 256;

extern const std::array<std::array<std::string, TARR_COLS>, TARR_ROWS> tarr;

} // namespace wertheim

#endif // WERTHEIM_TARR_DATA_HPP
