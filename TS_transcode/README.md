# WERTHEIM — Browser Encryption Tool / Böngészős Titkosító

**[EN]** A single HTML file. No installation, no npm, no dependencies.
Just open `wertheim.html` in any browser and start encrypting.

**[HU]** Egyetlen HTML fájl. Nincs telepítés, nincs npm, nincs függőség.
Csak nyisd meg `wertheim.html`-t bármely böngészőben, és máris használhatod.

---

## Quick Start / Gyors indítás

**[EN]**
1. Double-click `wertheim.html`
2. Enter a password
3. Type your text → **Encrypt**

**[HU]**
1. Dupla kattintás a `wertheim.html`-re
2. Írd be a jelszót
3. Írd be a szöveget → **Titkosítás**

---

## Compatibility / Kompatibilitás

**[EN]** Fully compatible with `.wtx` files from the original Pascal program:
what you encrypt with this HTML, the Pascal version can decrypt, and vice versa.

**[HU]** Teljes mértékben kompatibilis az eredeti Pascal program `.wtx` fájljaival:
amit ezzel a HTML-lel titkosítasz, azt a Pascal verzió is visszafejti, és fordítva.

---

## How It Works / Működés

- SHA-1 hash: Web Crypto API (browser built-in, hardware-accelerated)
- RC4 encryption: native JS implementation
- Custom "hinta" substitution cipher
- Custom Base64 (non-standard — Pascal compatible)

---

## Documentation / Dokumentáció

### English / Angol

| File | Description |
|------|-------------|
| `USER_MANUAL.en.md` | Complete user manual — all features explained |
| `TECHNICAL_DOCS.en.md` | Technical documentation — encryption pipeline, security analysis |
| `SOCIAL.en.md` | Short social media post (English) |

### Hungarian / Magyar

| Fájl | Leírás |
|------|--------|
| `USER_MANUAL.md` | Felhasználói kézikönyv — minden funkció részletes leírása |
| `TECHNICAL_DOCS.md` | Technikai dokumentáció — titkosítási lánc, komponensek, biztonság |
| `FACEBOOK.md` | Rövid ismertető Facebookra |

---

## Files / Fájlok

| File / Fájl | Role / Szerep |
|-------------|---------------|
| `wertheim.html` | **✅ The only file you need to open / Az egyetlen fájl, amit meg kell nyitni** |
| `src/` | TypeScript source (optional, for development) |
| `test/` | Tests (not needed for usage) |
| `CPP_transcode/` | C++ port (incomplete, no compiler available) |
