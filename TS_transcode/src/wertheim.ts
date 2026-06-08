/**
 * WERTHEIM Cipher — TypeScript (Browser) Port
 *
 * Complete port of the original Wertheim text
 * encryption tool. Dual-layer cipher:
 *   Layer 1: RC4 stream cipher, key = SHA1(password)
 *   Layer 2: Custom substitution cipher ("hinta") using 65×65
 *            permutation tables derived from the password
 *
 * This implementation runs entirely in the browser using:
 *   - Web Crypto API  for SHA-1 hashing
 *   - Native RC4 implementation (not exposed via Web Crypto API)
 *   - Custom Base64 (non‑standard alphabet order!)
 *
 * Usage:
 *   import { hintabe, hintaki } from "./wertheim.js";
 *   const enc = await hintabe("Hello", "password");
 *   const dec = await hintaki(enc, "password");
 */

import { tarr, TARR_ROWS } from "./tarr_data.js";

// ═══════════════════════════════════════════════════════════════════════════════
// CONSTANTS
// ═══════════════════════════════════════════════════════════════════════════════

/** Custom Base64 alphabet — WARNING: NOT standard Base64! */
export const CODES64 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

/** Hinta substitution alphabet (64 chars). */
export const ABC0    = "Q1Py876WF9OCczwXtIUqkjbDl+ZTuvoKNir4V/aRdxpAHg3hmSMsG2LEYB0Jne5f";

export const MAX_PWD_LEN = 50;

// ═══════════════════════════════════════════════════════════════════════════════
// CUSTOM BASE64  (non-standard alphabet order!)
// ═══════════════════════════════════════════════════════════════════════════════

export function encode64(s: string): string {
    let result = "";
    let a = 0, b = 0;
    for (let i = 0; i < s.length; i++) {
        const x = s.charCodeAt(i);
        b = b * 256 + x;
        a += 8;
        while (a >= 6) {
            a -= 6;
            result += CODES64[(b >> a) & 0x3F];
            b &= (1 << a) - 1;
        }
    }
    if (a > 0) {
        result += CODES64[(b << (6 - a)) & 0x3F];
    }
    return result;
}

export function decode64(s: string): string {
    let result = "";
    let a = 0, b = 0;
    for (let i = 0; i < s.length; i++) {
        const x = CODES64.indexOf(s[i]);
        if (x < 0) break;
        b = b * 64 + x;
        a += 6;
        if (a >= 8) {
            a -= 8;
            result += String.fromCharCode((b >> a) & 0xFF);
            b &= (1 << a) - 1;
        }
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════════
// RC4 IMPLEMENTATION  (manual — not exposed via Web Crypto API)
// ═══════════════════════════════════════════════════════════════════════════════

function rc4Crypt(key: Uint8Array, data: Uint8Array): Uint8Array {
    const s = new Uint8Array(256);
    for (let i = 0; i < 256; i++) s[i] = i;
    for (let i = 0, j = 0; i < 256; i++) {
        j = (j + s[i] + key[i % key.length]) & 0xFF;
        [s[i], s[j]] = [s[j], s[i]];
    }
    const out = new Uint8Array(data.length);
    let i = 0, j = 0;
    for (let k = 0; k < data.length; k++) {
        i = (i + 1) & 0xFF;
        j = (j + s[i]) & 0xFF;
        [s[i], s[j]] = [s[j], s[i]];
        out[k] = data[k] ^ s[(s[i] + s[j]) & 0xFF];
    }
    return out;
}

// ═══════════════════════════════════════════════════════════════════════════════
// SHA1 → RC4 KEY DERIVATION (uses Web Crypto API)
// ═══════════════════════════════════════════════════════════════════════════════

export async function besecure(mit: string, pw: string): Promise<string> {
    // SHA1 hash of password (Web Crypto API)
    const pwBytes = new TextEncoder().encode(pw);
    const sha1 = await crypto.subtle.digest("SHA-1", pwBytes);
    const key = new Uint8Array(sha1);

    // RC4 encrypt
    const inputBytes = new TextEncoder().encode(mit);
    const encBytes = rc4Crypt(key, inputBytes);
    // Return raw bytes as a latin1 string (each byte → one char)
    return new TextDecoder("latin1").decode(encBytes);
}

export async function kisecure(mit: string, pw: string): Promise<string> {
    // RC4 is symmetric; encryption == decryption
    return besecure(mit, pw);
}

// ═══════════════════════════════════════════════════════════════════════════════
// PASSWORD → KEY DERIVATION  (matches Pascal pw_to_kod + genhinta)
// ═══════════════════════════════════════════════════════════════════════════════

export function pw_to_kod(mit: string): { kod: number[]; pwdhossz: number; pwdcode: number } {
    const kod = new Array(MAX_PWD_LEN).fill(0);
    const l = Math.min(mit.length, MAX_PWD_LEN);
    let wossz = 0;
    for (let t = 0; t < l; t++) {
        const pos = ABC0.indexOf(mit[t]);
        kod[t] = pos >= 0 ? pos + 1 : 0;
        wossz += kod[t];
    }
    const pwdcode = wossz - Math.floor((Math.floor(wossz / l) / 10));
    return { kod, pwdhossz: l, pwdcode };
}

export function genhinta(pwd: string): { kod: number[]; hintar: string[]; pwdhossz: number; pwdcode: number } {
    // Step 1: Base64-encode password first
    const encoded = encode64(pwd);
    const trimmed = encoded.trim();

    // Step 2: Convert to kod
    const { kod, pwdhossz, pwdcode } = pw_to_kod(trimmed);

    // Step 3: Generate hintar rows
    const hintar: string[] = new Array(MAX_PWD_LEN).fill("");
    for (let t = 0; t < pwdhossz && t < MAX_PWD_LEN; t++) {
        const row = kod[t];
        const col = trimmed.charCodeAt(t);
        if (row >= 0 && row < TARR_ROWS) {
            hintar[t] = tarr[row][col] ?? "";
        }
    }
    return { kod, hintar, pwdhossz, pwdcode };
}

// ═══════════════════════════════════════════════════════════════════════════════
// CORE SUBSTITUTION CIPHER  ("hinta")
// ═══════════════════════════════════════════════════════════════════════════════

export function bekod(mit: string, hintar: string[], pwdhossz: number): string {
    let w = mit;
    for (let z = 0; z < 2; z++) {
        const x = w.length;
        let h = 0;
        let result = "";
        for (let q0 = 0; q0 < x; q0++) {
            const ch = w[q0];
            const i = ABC0.indexOf(ch);
            if (i >= 0 && h < hintar.length && i < hintar[h].length) {
                result += hintar[h][i] ?? ch;
            } else {
                result += ch;
            }
            h = (h + 1) % pwdhossz;
        }
        w = result;
    }
    return encode64(w);
}

export function kikod(mit: string, hintar: string[], pwdhossz: number): string {
    let w = decode64(mit);
    for (let z = 0; z < 2; z++) {
        const x = w.length;
        let h = 0;
        let result = "";
        for (let q0 = 0; q0 < x; q0++) {
            const ch = w[q0];
            const i = h < hintar.length ? hintar[h].indexOf(ch) : -1;
            if (i >= 0) {
                result += ABC0[i];
            } else {
                result += ch;
            }
            h = (h + 1) % pwdhossz;
        }
        w = result;
    }
    return w;
}

// ═══════════════════════════════════════════════════════════════════════════════
// FULL ENCODE / DECODE
// ═══════════════════════════════════════════════════════════════════════════════

export async function bekodol(mit: string, jelszo: string): Promise<string> {
    // Step 1: Generate hinta key
    const { hintar, pwdhossz } = genhinta(jelszo);

    // Step 2: RC4+SHA1 encrypt (returns latin1 string)
    const t1 = await besecure(mit, jelszo);

    // Step 3: Custom Base64 encode
    const t2 = encode64(t1);

    // Step 4: Hinta substitution encode
    return bekod(t2, hintar, pwdhossz);
}

export async function kikodol(mit: string, jelszo: string): Promise<string> {
    // Step 1: Generate hinta key
    const { hintar, pwdhossz } = genhinta(jelszo);

    // Step 2: Hinta substitution decode
    const t1 = kikod(mit, hintar, pwdhossz);

    // Step 3: Custom Base64 decode
    const t2 = decode64(t1);

    // Step 4: RC4+SHA1 decrypt
    return kisecure(t2, jelszo);
}

// ═══════════════════════════════════════════════════════════════════════════════
// PUBLIC API  (matches Pascal hintabe / hintaki)
// ═══════════════════════════════════════════════════════════════════════════════

/** Encrypt plaintext with password. Returns ciphertext as a string. */
export async function hintabe(mit: string, jelszo: string): Promise<string> {
    return bekodol(mit, jelszo);
}

/** Decrypt ciphertext with password. Returns plaintext as a string. */
export async function hintaki(mit: string, jelszo: string): Promise<string> {
    return kikodol(mit, jelszo);
}
