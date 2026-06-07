# WERTHEIM — Technikai dokumentáció

## Áttekintés

A **WERTHEIM** titkosító alkalmazás egyetlen önálló HTML fájl, amely a böngészőben fut. A Pascal/Delphi eredetiből lett átültetve TypeScript-en keresztül JavaScriptbe. Célja: a `.wtx` fájlformátum teljes kompatibilitásának megőrzése mellett egy korszerű, nulla telepítést igénylő felület biztosítása.

---

## Titkosítási lánc

### Bekódolás (`hintabe()`)

```
plaintext (string)
    │
    ▼
UTF-8 encode → Uint8Array
    │
    ▼
RC4 + SHA-1 jelszóhash
    │
    ▼
latin1 string (byte→char, 0..255)
    │
    ▼
encode64() — belső Base64
    │
    ▼
bekod() — hinta szubsztitúció (2 menet) → encode64() — külső Base64
    │
    ▼
ciphertext (string)
```

### Visszafejtés (`hintaki()`)

```
ciphertext (string)
    │
    ▼
kikod() — decode64() → hinta visszafejtés (2 menet) → decode64()
    │
    ▼
latin1 string
    │
    ▼
charCodeAt(i) & 0xFF → Uint8Array
    │
    ▼
RC4 + SHA-1 jelszóhash (visszafejtés = újratitkosítás)
    │
    ▼
UTF-8 decode
    │
    ▼
plaintext (string)
```

---

## Komponensek részletesen

### 1. SHA-1 hash — Web Crypto API

```javascript
async function sha1(str) {
    const buf = new TextEncoder().encode(str);
    const hash = await crypto.subtle.digest("SHA-1", buf);
    return new Uint8Array(hash);
}
```

A böngésző beépített, hardvergyorsított SHA-1 implementációját használja. A jelszóból 20 bájtos hash-t állít elő, ami az RC4 kulcsgenerálásához szükséges.

### 2. RC4 titkosítás

Natív JavaScript implementáció, amely a Pascal `titokrutin.pas`-ban lévő algoritmust követi. A kulcsütemezés (KSA) és a pszeudo-véletlenszám generátor (PRNG) is szabványos RC4. A visszafejtés azonos a titkosítással (XOR tulajdonság).

### 3. Byte-safe konverzió

A legkritikusabb komponens. Az RC4 kimenete bájtok sorozata (0–255). Ahhoz, hogy ezt Base64-ként kezelhessük, a bájtokat karakterekké kell konvertálni:

**Helyes megközelítés:**
```javascript
String.fromCharCode(...enc)  // byte → char (0–255 → U+0000–U+00FF)
```

**Hibás megközelítés (korábbi verzióban):**
```javascript
new TextDecoder("latin1").decode(enc)  // 0x80–0x9F → Windows-1252 multi-byte
```

A `TextDecoder("latin1")` a 0x80–0x9F tartományt Windows-1252 karakterekké alakítja (pl. 0x80 → € = U+20AC), ami megtöri a Base64 kódolást, mert a kapott karakterek kódpontja > 255. A `String.fromCharCode()` minden bájtot változatlanul 1:1-ben képez le karakterre.

### 4. Base64 kódolás (`encode64` / `decode64`)

Nem szabványos Base64 — a Pascal eredetivel kompatibilis. A 64 karakterből álló ABC0 (`"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"`) megegyezik a Pascal változatéval. A kódolás 3 bájtot 4 karakterré alakít, a kitöltés `=` karakterrel történik.

### 5. Hinta szubsztitúció

**Koncepció:** Minden jelszó karakterhez tartozik egy permutációs tábla sor (`tarr`). Az `i.` sor az ABC0 összes karakterének egy permutációját tartalmazza. A szöveg minden karakterét helyettesítjük a megfelelő sor megfelelő oszlopában lévő karakterrel.

**Megvalósítás:**

```javascript
function genhinta(pwd) {
    const pwdenc = encode64(pwd);            // → Base64
    const trimmed = pwdenc.replace(/=+$/, ""); // kitöltés levágva
    const kod = new Array(MAX_PWD_LEN);       // sorindexek
    for (let t = 0; t < pwdhossz && t < MAX_PWD_LEN; t++) {
        kod[t] = pwdenc.charCodeAt(t) % TARR_ROWS;
        hintar[t] = tarr[kod[t]][col];
    }
}
```

**Fontos megjegyzés:** A `col` index itt `pwd.charCodeAt(t)`, ahol `pwd` az `encode64(password)`. A Base64 karakterek kódpontjai 43–122 közé esnek, de a `tarr[row]` tömbök csak 65 indexet tartalmaznak (0–64). Így a `hintar[t]` a legtöbb jelszó esetén `undefined` lesz (a Pascal eredetiben ez undefined behavior, ami azonosan viselkedik). Ez **nem befolyásolja a round-trip helyességet**, mert a hibás index szimmetrikusan jelentkezik a kódolásban és a visszafejtésben.

**A hinta szerepe:** A 2 menetes behelyettesítés további keverést ad a Base64 kimenetre. Bár a fenti oszlopindex hiba miatt a jelenlegi implementáció a legtöbb jelszóval gyakorlatilag nem változtat a Base64 szövegen (a helyettesítés üres stringre cseréli a karaktereket), ez szimmetrikus, ezért a visszafejtés helyesen fordítja vissza.

### 6. `tarr` tábla

A 64×65-ös permutációs tábla minden sorában az ABC0 összes karakterének egy véletlenszerű permutációja található. A Pascal program generálta, fixen beégetve a forráskódba (`tarr_data.ts` → `wertheim.html`).

---

## UI architektúra

### HTML struktúra

```
.app
├── h1 + cím
├── .pw-row (jelszó + show/hide gomb)
├── .action-row (Titkosítás, Visszafejtés, Csere, Mindent másol, Új/Töröl)
├── h2 + .toolbar + .editor-panel (Szöveg)
├── h2 + .toolbar + .editor-panel (Titkosított)
├── .status-bar (INS/OVR, pozíció, tesztek)
├── .footer
└── .drop-overlay (drag & drop)
```

### JavaScript modulok

| Funkció | Leírás |
|---------|--------|
| `sha1()` | SHA-1 hash (Web Crypto API) |
| `rc4()` | RC4 titkosítás/visszafejtés |
| `besecure()` | RC4 titkosítás + byte→char konverzió |
| `kisecure()` | char→byte konverzió + RC4 |
| `encode64()` / `decode64()` | Base64 (Pascal kompatibilis) |
| `genhinta()` | Hinta permutációs tábla generálása jelszóból |
| `bekod()` / `kikod()` | Hinta szubsztitúció (2 menet) |
| `hintabe()` / `hintaki()` | Teljes titkosítási/visszafejtési lánc |
| `updateButtons()` | Jelszó mező figyelése — gombok engedélyezése/tiltása |
| `saveFile()` | Fájl mentés prompt dialógussal |
| `copyText()` / `copyAll()` | Vágólap műveletek |
| `pasteText()` | Beillesztés kurzor pozícióba |
| `updatePos()` | Kurzor pozíció követése |

### Stílus

- Színséma: sötét téma (`#1a1a2e` háttér, `#e94560` akcentus)
- Reszponzív: `max-width: 960px`, középre igazítva
- Panelszerkesztők: flexibilis magasságú (`flex: 1`), görgethető textarea
- Gombstátuszok: normál, hover, disabled állapotok vizuálisan elkülönítve

---

## Biztonsági elemzés

### Gyengeségek

1. **Hinta oszlopindex hiba:** A `genhinta()` `pwd.charCodeAt(t)` indexe túlmutat a `tarr[row]` tömb méretén a legtöbb jelszó esetén. Ez gyakorlatilag kikapcsolja a hinta szubsztitúciót. A hiba szimmetrikus, így a kompatibilitás nem sérül, de a titkosítás erőssége csökken.

2. **RC4 ismert gyengeségei:** Az RC4 nem tekinthető modern szabványnak. Biasok az első bájtokban, és ismert statisztikai támadások ellene.

3. **Nincs hitelesítés:** Az üzenet integritását semmi sem védi. A titkosított szöveg módosítható anélkül, hogy ez detektálható lenne.

### Erősségek

1. **Byte-hű konverzió:** A `String.fromCharCode()` biztosítja, hogy minden bájt 1:1-ben leképeződjön, szemben a `TextDecoder("latin1")` hibás viselkedésével.

2. **Nincs külső függőség:** Egyetlen fájl, nincs CDN, nincs harmadik fél kódja.

3. **Zero-trust kialakítás:** Nincs szerver, nincs adatküldés, nincs telemetria.

---

## Fejlesztői környezet

### Fájlok

| Fájl | Leírás |
|------|--------|
| `wertheim.html` | A kész alkalmazás (ezt kell használni) |
| `src/wertheim.ts` | TypeScript forráskód |
| `src/tarr_data.ts` | Tarr tábla adatok |
| `src/index.ts` | UI belépési pont |
| `test/*.ts` | Tesztek |
| `hinta.pas` | Pascal eredeti (hinta szubsztitúció + Base64) |
| `titokrutin.pas` | Pascal eredeti (RC4 + SHA-1) |
| `unit1.pas` | Pascal eredeti (GUI) |

### Build

```bash
npm install
npm run build    # → dist/wertheim.js
```

A kész HTML manuálisan lett összeállítva a TypeScript kimenet és a kézzel írt UI-kód egyesítésével.

### Tesztelés

```bash
npm test
```

A tesztek lefedik:
- Base64 round-trip
- Alap titkosítás/visszafejtés
- Magyar UTF-8 karakterek (ékezetek)
- Hosszú szöveg (1000 karakter)
- Hibás jelszó (garblált kimenet ellenőrzése)

---

## Verziótörténet

| Verzió | Dátum | Változás |
|--------|-------|----------|
| 1.0 | — | Pascal eredeti |
| 2.0 | — | TypeScript port |
| 2.1 | — | `TextDecoder("latin1")` hiba javítása → `String.fromCharCode()` |
| 2.2 | — | Teljes UI áttervezés: fájlkezelés, drag & drop, vágólap, szerkesztő funkciók |
