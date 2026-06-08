# WERTHEIM — Technische Dokumentation

## Übersicht

**WERTHEIM** ist eine eigenständige HTML-Datei, die als Textverschlüsselungswerkzeug im Browser läuft. Es wurde vom ursprünglichen Pascal/Delphi-Programm über TypeScript nach JavaScript portiert. Ziel ist die vollständige Kompatibilität mit dem `.wtx`-Dateiformat bei gleichzeitiger Bereitstellung einer modernen, installationsfreien Benutzeroberfläche.

---

## Verschlüsselungspipeline

### Kodierung (`hintabe()`)

```
plaintext (string)
    │
    ▼
UTF-8 kodieren → Uint8Array
    │
    ▼
RC4 + SHA-1 Passwort-Hash
    │
    ▼
latin1-String (byte→char, 0..255)
    │
    ▼
encode64() — interne Base64
    │
    ▼
bekod() — Hinta-Substitution (2 Durchgänge) → encode64() — externe Base64
    │
    ▼
ciphertext (string)
```

### Dekodierung (`hintaki()`)

```
ciphertext (string)
    │
    ▼
kikod() — decode64() → Hinta-Rücksubstitution (2 Durchgänge) → decode64()
    │
    ▼
latin1-String
    │
    ▼
charCodeAt(i) & 0xFF → Uint8Array
    │
    ▼
RC4 + SHA-1 Passwort-Hash (Entschlüsselung = erneute Verschlüsselung)
    │
    ▼
UTF-8 dekodieren
    │
    ▼
plaintext (string)
```

---

## Komponenten im Detail

### 1. SHA-1-Hash — Web Crypto API

```javascript
async function sha1(str) {
    const buf = new TextEncoder().encode(str);
    const hash = await crypto.subtle.digest("SHA-1", buf);
    return new Uint8Array(hash);
}
```

Nutzt die browserinterne, hardwarebeschleunigte SHA-1-Implementierung. Erzeugt aus dem Passwort einen 20-Byte-Hash, der für die RC4-Schlüsselgenerierung benötigt wird.

### 2. RC4-Verschlüsselung

Native JavaScript-Implementierung, die dem Algorithmus aus `titokrutin.pas` (Pascal) folgt. Sowohl die Schlüsselaufbereitung (KSA) als auch der Pseudozufallszahlengenerator (PRNG) entsprechen dem Standard-RC4. Die Entschlüsselung ist identisch mit der Verschlüsselung (XOR-Eigenschaft).

### 3. Byte-sichere Konvertierung

Die kritischste Komponente. Die RC4-Ausgabe ist eine Folge von Bytes (0–255). Um diese als Base64 zu behandeln, müssen die Bytes in Zeichen umgewandelt werden:

**Korrekte Methode:**
```javascript
String.fromCharCode(...enc)  // byte → char (0–255 → U+0000–U+00FF)
```

**Falsche Methode (in früheren Versionen):**
```javascript
new TextDecoder("latin1").decode(enc)  // 0x80–0x9F → Windows-1252 Multibyte
```

`TextDecoder("latin1")` wandelt den Bereich 0x80–0x9F in Windows-1252-Zeichen um (z. B. 0x80 → € = U+20AC), was die Base64-Kodierung zerstört, da die resultierenden Codepoints > 255 sind. `String.fromCharCode()` bildet jedes Byte unverändert 1:1 auf ein Zeichen ab.

### 4. Base64-Kodierung (`encode64` / `decode64`)

Keine Standard-Base64 – kompatibel mit dem Pascal-Original. Das 64-Zeichen-Alphabet ABC0 (`"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"`) entspricht dem der Pascal-Version. Die Kodierung wandelt 3 Bytes in 4 Zeichen um, Padding erfolgt mit `=`.

### 5. Hinta-Substitution

**Konzept:** Zu jedem Passwortzeichen gehört eine Zeile in einer Permutationstabelle (`tarr`). Die `i.` Zeile enthält eine Permutation aller Zeichen des ABC0. Jedes Zeichen des Texts wird durch das Zeichen aus der entsprechenden Zeile und Spalte ersetzt.

**Implementierung:**

```javascript
function genhinta(pwd) {
    const pwdenc = encode64(pwd);            // → Base64
    const trimmed = pwdenc.replace(/=+$/, ""); // Padding entfernt
    const kod = new Array(MAX_PWD_LEN);       // Zeilenindizes
    for (let t = 0; t < pwdhossz && t < MAX_PWD_LEN; t++) {
        kod[t] = pwdenc.charCodeAt(t) % TARR_ROWS;
        hintar[t] = tarr[kod[t]][col];
    }
}
```

**Wichtige Anmerkung:** Der `col`-Index ist hier `pwd.charCodeAt(t)`, wobei `pwd` das `encode64(password)` ist. Die Codepoints der Base64-Zeichen liegen zwischen 43 und 122, aber die `tarr[row]`-Arrays enthalten nur 65 Indizes (0–64). Daher ist `hintar[t]` bei den meisten Passwörtern `undefined` (im Pascal-Original undefiniertes Verhalten, das sich identisch verhält). Dies **beeinträchtigt nicht die Round-Trip-Korrektheit**, da der fehlerhafte Index symmetrisch bei Kodierung und Dekodierung auftritt.

**Rolle der Hinta:** Die 2-durchgängige Substitution fügt der Base64-Ausgabe eine zusätzliche Durchmischung hinzu. Aufgrund des oben genannten Spaltenindexfehlers ändert die aktuelle Implementierung bei den meisten Passwörtern praktisch nichts am Base64-Text (die Substitution ersetzt Zeichen durch leere Strings), aber da dies symmetrisch ist, funktioniert die Entschlüsselung korrekt.

### 6. `tarr`-Tabelle

Die 64×65-Permutationstabelle enthält in jeder Zeile eine zufällige Permutation aller Zeichen des ABC0. Sie wurde vom Pascal-Programm generiert und ist fest im Quellcode eingebettet (`tarr_data.ts` → `wertheim.html`).

---

## UI-Architektur

### HTML-Struktur

```
.app
├── h1 + Titel
├── .pw-row (Passwort + Ein-/Ausblenden-Button)
├── .action-row (Verschlüsseln, Entschlüsseln, Tauschen, Alles kopieren, Neu/Löschen)
├── h2 + .toolbar + .editor-panel (Text)
├── h2 + .toolbar + .editor-panel (Verschlüsselt)
├── .status-bar (INS/OVR, Position, Tests)
├── .footer
└── .drop-overlay (Drag & Drop)
```

### JavaScript-Module

| Funktion | Beschreibung |
|----------|--------------|
| `sha1()` | SHA-1-Hash (Web Crypto API) |
| `rc4()` | RC4-Verschlüsselung/-Entschlüsselung |
| `besecure()` | RC4-Verschlüsselung + Byte→Char-Konvertierung |
| `kisecure()` | Char→Byte-Konvertierung + RC4 |
| `encode64()` / `decode64()` | Base64 (Pascal-kompatibel) |
| `genhinta()` | Hinta-Permutationstabelle aus Passwort generieren |
| `bekod()` / `kikod()` | Hinta-Substitution (2 Durchgänge) |
| `hintabe()` / `hintaki()` | Vollständige Ver-/Entschlüsselungskette |
| `updateButtons()` | Überwacht Passwortfeld – aktiviert/deaktiviert Schaltflächen |
| `saveFile()` | Datei speichern mit Dialog |
| `copyText()` / `copyAll()` | Zwischenablage-Operationen |
| `pasteText()` | Einfügen an Cursorposition |
| `updatePos()` | Cursorposition verfolgen |

### Stil

- Farbschema: Dunkles Design (`#1a1a2e` Hintergrund, `#e94560` Akzent)
- Responsiv: `max-width: 960px`, zentriert
- Panel-Editoren: flexible Höhe (`flex: 1`), scrollbare Textarea
- Schaltflächenzustände: Normal, Hover, Deaktiviert visuell getrennt

---

## Sicherheitsanalyse

### Schwächen

1. **Hinta-Spaltenindexfehler:** Der `pwd.charCodeAt(t)`-Index in `genhinta()` liegt bei den meisten Passwörtern außerhalb der `tarr[row]`-Arraygrenzen. Dies deaktiviert die Hinta-Substitution praktisch. Der Fehler ist symmetrisch, sodass die Kompatibilität erhalten bleibt, aber die Verschlüsselungsstärke verringert sich.

2. **Bekannte RC4-Schwächen:** RC4 gilt nicht als moderner Standard. Es gibt Biases in den ersten Bytes und bekannte statistische Angriffe.

3. **Keine Authentifizierung:** Die Nachrichtenintegrität ist nicht geschützt. Der verschlüsselte Text kann unbemerkt modifiziert werden.

### Stärken

1. **Bytegetreue Konvertierung:** `String.fromCharCode()` stellt sicher, dass jedes Byte 1:1 abgebildet wird, im Gegensatz zum fehlerhaften Verhalten von `TextDecoder("latin1")`.

2. **Keine externen Abhängigkeiten:** Einzelne Datei, kein CDN, kein Drittanbieter-Code.

3. **Zero-Trust-Design:** Kein Server, kein Datenversand, keine Telemetrie.

---

## Entwicklerumgebung

### Dateien

| Datei | Beschreibung |
|-------|--------------|
| `wertheim.html` | Die fertige Anwendung (zu verwenden) |
| `src/wertheim.ts` | TypeScript-Quellcode |
| `src/tarr_data.ts` | Tarr-Tabellendaten |
| `src/index.ts` | UI-Einstiegspunkt |
| `test/*.ts` | Tests |
| `hinta.pas` | Pascal-Original (Hinta-Substitution + Base64) |
| `titokrutin.pas` | Pascal-Original (RC4 + SHA-1) |
| `unit1.pas` | Pascal-Original (GUI) |

### Build

```bash
npm install
npm run build    # → dist/wertheim.js
```

Das fertige HTML wurde manuell aus der TypeScript-Ausgabe und dem handgeschriebenen UI-Code zusammengesetzt.

### Tests

```bash
npm test
```

Die Tests decken ab:
- Base64-Round-Trip
- Grundlegende Ver- und Entschlüsselung
- Ungarische UTF-8-Zeichen (Akzente)
- Lange Texte (1000 Zeichen)
- Falsches Passwort (Überprüfung auf verstümmelte Ausgabe)

---

## Versionsgeschichte

| Version | Datum | Änderung |
|---------|-------|----------|
| 1.0 | — | Pascal-Original |
| 2.0 | — | TypeScript-Port |
| 2.1 | — | `TextDecoder("latin1")`-Fehler behoben → `String.fromCharCode()` |
| 2.2 | — | Komplettes UI-Redesign: Dateiverwaltung, Drag & Drop, Zwischenablage, Editorfunktionen |
