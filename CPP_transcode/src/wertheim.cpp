#include "wertheim.hpp"
#include "wertheim_tarr_data.hpp"

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <openssl/evp.h>
#include <openssl/rc4.h>

namespace wertheim {

// ═══════════════════════════════════════════════════════════════
// CONSTANTS
// ═══════════════════════════════════════════════════════════════

const std::string CODES64 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
const std::string ABC0    = "Q1Py876WF9OCczwXtIUqkjbDl+ZTuvoKNir4V/aRdxpAHg3hmSMsG2LEYB0Jne5f";
const int MAX_PWD_LEN = 50;

// ═══════════════════════════════════════════════════════════════
// CUSTOM BASE64 (non-standard alphabet order!)
// ═══════════════════════════════════════════════════════════════

std::string encode64(const std::string& s) {
    std::string result;
    int a = 0, b = 0;
    for (size_t i = 0; i < s.size(); i++) {
        int x = static_cast<unsigned char>(s[i]);
        b = b * 256 + x;
        a += 8;
        while (a >= 6) {
            a -= 6;
            x = b >> a;
            b &= (1 << a) - 1;
            result += CODES64[x];
        }
    }
    if (a > 0) {
        int x = b << (6 - a);
        result += CODES64[x];
    }
    return result;
}

std::string decode64(const std::string& s) {
    std::string result;
    int a = 0, b = 0;
    for (size_t i = 0; i < s.size(); i++) {
        int x = static_cast<int>(CODES64.find(s[i]));
        if (x >= 0) {
            b = b * 64 + x;
            a += 6;
            if (a >= 8) {
                a -= 8;
                x = b >> a;
                b &= (1 << a) - 1;
                x &= 255;
                result += static_cast<char>(x);
            }
        } else {
            break;
        }
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════
// PASSWORD → KEY DERIVATION
// ═══════════════════════════════════════════════════════════════

void pw_to_kod(const std::string& mit, int kod[50], int& pwdhossz, int& pwdcode) {
    for (int t = 0; t < MAX_PWD_LEN; t++) kod[t] = 0;

    int l = static_cast<int>(mit.length());
    if (l > MAX_PWD_LEN) l = MAX_PWD_LEN;

    int wossz = 0;
    for (int t = 0; t < l; t++) {
        // Find character position in abc0 (1-based in Pascal, 0-based here)
        size_t pos = ABC0.find(mit[t]);
        kod[t] = (pos != std::string::npos) ? static_cast<int>(pos) + 1 : 0;
        wossz += kod[t];
    }
    pwdcode = wossz - ((wossz / l) / 10);
    pwdhossz = l;
}

void genhinta(const std::string& pwd, int kod[50], std::string hintar[50], int& pwdhossz, int& pwdcode) {
    // Step 1: Base64-encode the password first
    std::string encoded = encode64(pwd);
    // Pascal's trim() - remove leading/trailing whitespace
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = encoded.find_first_not_of(whitespace);
    size_t end = encoded.find_last_not_of(whitespace);
    std::string trimmed = (start == std::string::npos) ? "" : encoded.substr(start, end - start + 1);

    // Step 2: Convert to kod array
    pw_to_kod(trimmed, kod, pwdhossz, pwdcode);

    // Step 3: Generate hintar rows from tarr table
    // NOTE: This replicates the Pascal code's tarr[kod[t], ord(pwd[t])] access.
    // In Pascal, tarr is [0..64, 0..64] of string, but ord(pwd[t]) can be > 64
    // for base64 characters like 'A' (65) or 'z' (122).
    // The C++ port uses tarr[65][256] and treats uninitialized entries as "".
    for (int t = 0; t < pwdhossz && t < MAX_PWD_LEN; t++) {
        int row = kod[t];
        unsigned char col = static_cast<unsigned char>(trimmed[t]);
        if (row >= 0 && row < TARR_ROWS) {
            hintar[t] = tarr[row][static_cast<size_t>(col)];
        } else {
            hintar[t] = "";
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// CORE SUBSTITUTION CIPHER
// ═══════════════════════════════════════════════════════════════

std::string bekod(const std::string& mit, const std::string hintar[50], int pwdhossz) {
    // Encode substitution: for each char, find it in abc0, replace with
    // the char from the current hintar row at that position.
    // Repeated for 2 passes (for z:=1 to 2 do)
    std::string w = mit;
    for (int z = 0; z < 2; z++) {
        int x = static_cast<int>(w.length());
        int h = 0;  // 0-based index into hintar
        std::string result;
        result.reserve(x);
        for (int q0 = 0; q0 < x; q0++) {
            std::string q1(1, w[q0]);
            size_t i = ABC0.find(q1);  // position in abc0
            if (i != std::string::npos && !hintar[h].empty() && i < hintar[h].length()) {
                result += hintar[h][i];
            } else {
                result += q1; // fallback: unchanged
            }
            h = (h + 1) % pwdhossz;
        }
        w = result;
    }
    return encode64(w);
}

std::string kikod(const std::string& mit, const std::string hintar[50], int pwdhossz) {
    // Decode substitution: reverse of bekod
    std::string w = decode64(mit);
    for (int z = 0; z < 2; z++) {
        int x = static_cast<int>(w.length());
        int h = 0;
        std::string result;
        result.reserve(x);
        for (int q0 = 0; q0 < x; q0++) {
            std::string q1(1, w[q0]);
            // Find q1 in the current hintar row
            size_t i = std::string::npos;
            if (!hintar[h].empty()) {
                i = hintar[h].find(q1);
            }
            if (i != std::string::npos) {
                result += ABC0[i];
            } else {
                result += q1; // fallback
            }
            h = (h + 1) % pwdhossz;
        }
        w = result;
    }
    return w;
}

// ═══════════════════════════════════════════════════════════════
// RC4 + SHA1 LAYER  (OpenSSL EVP)
// ═══════════════════════════════════════════════════════════════

std::string besecure(const std::string& mit, const std::string& pw) {
    // SHA1 hash of password → RC4 key
    unsigned char sha1_digest[EVP_MAX_MD_SIZE];
    unsigned int sha1_len = 0;
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha1(), nullptr);
    EVP_DigestUpdate(mdctx, pw.data(), pw.size());
    EVP_DigestFinal_ex(mdctx, sha1_digest, &sha1_len);
    EVP_MD_CTX_free(mdctx);

    // RC4 encryption using the SHA1 hash as key
    RC4_KEY key;
    RC4_set_key(&key, sha1_len, sha1_digest);

    std::string result(mit.size(), '\0');
    RC4(&key, mit.size(),
        reinterpret_cast<const unsigned char*>(mit.data()),
        reinterpret_cast<unsigned char*>(result.data()));

    return result;
}

std::string kisecure(const std::string& mit, const std::string& pw) {
    // RC4 is symmetric - encryption and decryption are the same operation
    return besecure(mit, pw);
}

// ═══════════════════════════════════════════════════════════════
// FULL ENCODE / DECODE (hinta layer + RC4 layer)
// ═══════════════════════════════════════════════════════════════

static void ensure_globals_initialized() {
    // Force tarr data load (it's already static init, this is a no-op)
    // but ensures the tarr table is referenced
    static_cast<void>(tarr[0][0]);
}

std::string bekodol(const std::string& mit, const std::string& jelszo,
                    int kod[50], std::string hintar[50], int& pwdhossz, int& pwdcode) {
    ensure_globals_initialized();

    // Step 1: Generate hinta key from password
    genhinta(jelszo, kod, hintar, pwdhossz, pwdcode);

    // Step 2: RC4+SHA1 encrypt
    std::string t1 = besecure(mit, jelszo);

    // Step 3: Custom Base64 encode
    std::string t2 = encode64(t1);

    // Step 4: Hinta substitution encode
    std::string t3 = bekod(t2, hintar, pwdhossz);

    return t3;
}

std::string kikodol(const std::string& mit, const std::string& jelszo,
                    int kod[50], std::string hintar[50], int& pwdhossz, int& pwdcode) {
    ensure_globals_initialized();

    // Step 1: Generate hinta key from password
    genhinta(jelszo, kod, hintar, pwdhossz, pwdcode);

    // Step 2: Hinta substitution decode
    std::string t1 = kikod(mit, hintar, pwdhossz);

    // Step 3: Custom Base64 decode
    std::string t2 = decode64(t1);

    // Step 4: RC4+SHA1 decrypt
    std::string t3 = kisecure(t2, jelszo);

    return t3;
}

// ═══════════════════════════════════════════════════════════════
// PUBLIC API
// ═══════════════════════════════════════════════════════════════

std::string hintabe(const std::string& mit, const std::string& jelszo) {
    int kod[50] = {0};
    std::string hintar[50];
    int pwdhossz = 0, pwdcode = 0;
    return bekodol(mit, jelszo, kod, hintar, pwdhossz, pwdcode);
}

std::string hintaki(const std::string& mit, const std::string& jelszo) {
    int kod[50] = {0};
    std::string hintar[50];
    int pwdhossz = 0, pwdcode = 0;
    return kikodol(mit, jelszo, kod, hintar, pwdhossz, pwdcode);
}

} // namespace wertheim
