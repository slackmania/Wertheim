<p align="center">
  <img src="https://img.shields.io/badge/status-stable-brightgreen" alt="Status">
  <img src="https://img.shields.io/badge/license-MIT-blue" alt="License">
  <img src="https://img.shields.io/badge/platform-browser-important" alt="Platform">
  <img src="https://img.shields.io/badge/size-95%20kB-lightgrey" alt="Size">
  <img src="https://img.shields.io/badge/🌐-6%20languages-important" alt="Languages">
</p>

<h1 align="center">🔐 WERTHEIM</h1>
<p align="center"><b>Browser-based text encryption</b><br>
Zero-install · No server · .wtx compatible</p>

---

A single **standalone HTML file** — no build tools, no npm, no dependencies. Double-click and encrypt.

```
─────  Encryption Pipeline  ─────
Plaintext → UTF-8 → RC4 + SHA-1 → 
Base64 → Hinta substitution → Ciphertext (.wtx)
```

---

## ✨ Features

| Feature | |
|---------|-|
| **🔒 Encrypt / 🔓 Decrypt** | Password-based text encryption |
| **📂 File I/O** | Open and save `.txt` / `.wtx` files |
| **📋 Clipboard** | Copy, paste, copy-all with headers |
| **📎 Drag & Drop** | Drop files anywhere on the page |
| **🔄 .wtx auto-decode** | Opens and decrypts `.wtx` files in one step |
| **⌨️ Full editor** | Tab=3 spaces, INS/OVR toggle, line/col tracking |

---

## 🚀 Get started

```
1. Download  wertheim.html
2. Double-click to open in your browser
3. Enter a password
4. Type text → click  🔒 Encrypt
```

That's it. Everything runs locally — **no data ever leaves your machine**.

---

## 🔧 Technical stack

| Component | Implementation |
|-----------|---------------|
| SHA-1 | Web Crypto API (hardware-accelerated) |
| Encryption | RC4 (native JS) |
| Substitution | "Hinta" — custom permutation cipher (2 rounds) |
| Encoding | Custom Base64 (Pascal-compatible alphabet) |
| Language | JavaScript in a single `.html` file |
| Source | TypeScript in `src/` |

---

## 🌐 Languages / Nyelvek

| Flag | Language | UI | Manual | Technical | Social |
|------|----------|:--:|:------:|:---------:|:------:|
| 🇭🇺 | Magyar | ✅ | [📖](TS_transcode/USER_MANUAL.md) | [🔧](TS_transcode/TECHNICAL_DOCS.md) | [📣](TS_transcode/FACEBOOK.md) |
| 🇬🇧 | English | ✅ | [📖](TS_transcode/USER_MANUAL.en.md) | [🔧](TS_transcode/TECHNICAL_DOCS.en.md) | [📣](TS_transcode/SOCIAL.en.md) |
| 🇩🇪 | Deutsch | ✅ | [📖](TS_transcode/USER_MANUAL.de.md) | [🔧](TS_transcode/TECHNICAL_DOCS.de.md) | [📣](TS_transcode/SOCIAL.de.md) |
| 🇫🇷 | Français | ✅ | [📖](TS_transcode/USER_MANUAL.fr.md) | [🔧](TS_transcode/TECHNICAL_DOCS.fr.md) | [📣](TS_transcode/SOCIAL.fr.md) |
| 🇷🇺 | Русский | ✅ | [📖](TS_transcode/USER_MANUAL.ru.md) | [🔧](TS_transcode/TECHNICAL_DOCS.ru.md) | [📣](TS_transcode/SOCIAL.ru.md) |
| 🇸🇦 | العربية | ✅ | [📖](TS_transcode/USER_MANUAL.ar.md) | [🔧](TS_transcode/TECHNICAL_DOCS.ar.md) | [📣](TS_transcode/SOCIAL.ar.md) |

---

## 📁 Repository structure

```
wertheim/
├── wertheim.html          ← The app (just open this)
├── TS_transcode/
│   ├── src/               TypeScript source
│   ├── test/              Tests (npm test)
│   ├── USER_MANUAL.md     User manual (HU)
│   ├── USER_MANUAL.en.md  User manual (EN)
│   ├── TECHNICAL_DOCS.md  Technical docs (HU)
│   ├── TECHNICAL_DOCS.en.md Technical docs (EN)
│   └── FACEBOOK.md        Social post (HU)
├── CPP_transcode/         C++ port (incomplete)
├── hinta.pas              Pascal original
├── titokrutin.pas         Pascal original
└── unit1.pas              Pascal original
```

---

## 📚 Documentation

- [User Manual (EN)](TS_transcode/USER_MANUAL.en.md)
- [User Manual (HU)](TS_transcode/USER_MANUAL.md)
- [Technical Documentation (EN)](TS_transcode/TECHNICAL_DOCS.en.md)
- [Technical Documentation (HU)](TS_transcode/TECHNICAL_DOCS.md)

---

## 🧪 Tests

Open the app, click **🧪 Tesztek** in the status bar, or run:

```bash
cd TS_transcode
npm install
npm test
```

All 5 tests pass: Base64 round-trip · Basic encrypt/decrypt · Hungarian UTF-8 · Long text (1000 chars) · Wrong password rejection.

---

## 🤝 Contributing

PRs welcome. The main deliverable is `wertheim.html` — the TypeScript sources are in `src/` and are compiled into it manually.

---

## 📜 License

MIT — do what you want with it.

---

<p align="center"><i>Built from the original Pascal program.</i></p>
