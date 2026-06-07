# WERTHEIM Encryptor — User Manual

## 1. Introduction

**WERTHEIM** is a browser-based text encryption application. You can encrypt any text with a password, save the encrypted text to a file, send it, and the recipient can decrypt it with the correct password.

**Important:** All encryption and decryption happens on your machine. Nothing leaves your browser — no server, no data upload.

---

## 2. Installation

**No installation required.** Just open a single HTML file:

1. Download `wertheim.html`
2. Double-click it — it opens in your browser
3. Ready to use immediately

**Supported browsers:** Chrome, Brave, Edge, Firefox, Safari, Opera — any modern browser.

---

## 3. Interface Overview

```
┌─────────────────────────────────────┐
│          🔐 WERTHEIM                │
│  Text Encryptor                     │
├─────────────────────────────────────┤
│  [Password field]          [👁]     │
├─────────────────────────────────────┤
│  [🔒 Encrypt] [🔓 Decrypt]         │
│  [⇅ Swap] [📋 Copy All]            │
│  [🗑 New / Clear]                   │
├─ "Plaintext" panel ─────────────────┤
│  [📂 Open] [💾 Save]                │
│  [📋 Copy] [📋 Paste]               │
│  [🗑 Clear]                          │
│  ┌────────────────────────────────┐ │
│  │ Enter text to encrypt...      │ │
│  └────────────────────────────────┘ │
├─ "Ciphertext" panel ────────────────┤
│  [📂 Open] [💾 Save]                │
│  [📋 Copy] [📋 Paste]               │
│  [🗑 Clear]                          │
│  ┌────────────────────────────────┐ │
│  │ ...encrypted text appears      │ │
│  │ here...                        │ │
│  └────────────────────────────────┘ │
├─────────────────────────────────────┤
│  INS          Line: 1  Col: 1       │
└─────────────────────────────────────┘
```

### Main Elements

| Element | Description |
|---------|-------------|
| **Password field** | Enter your password here. Buttons are disabled without it. |
| **👁** | Show/hide password |
| **🔒 Encrypt** | Encrypts the top panel text, result goes to bottom panel |
| **🔓 Decrypt** | Decrypts the bottom panel text, result goes to top panel |
| **⇅ Swap** | Swaps the content of the two panels |
| **📋 Copy All** | Copies both panels' content to clipboard (with headers) |
| **🗑 New / Clear** | Clears both panels (with confirmation) |

### Panel Toolbar (both panels):

| Button | Description |
|--------|-------------|
| **📂 Open** | Load a file from your computer |
| **💾 Save** | Save panel content to a file (prompts for filename) |
| **📋 Copy** | Copy panel content to clipboard |
| **📋 Paste** | Paste clipboard content into panel (at cursor position) |
| **🗑 Clear** | Clear panel content |

### Status Bar:

| Element | Description |
|---------|-------------|
| **INS / OVR** | Insert mode (INS) / Overwrite mode (OVR) — toggle with **Insert** key |
| **Line: / Col:** | Current cursor position |
| **🧪 Tests** | Run automated tests (results shown for 8 seconds) |

---

## 4. Basic Usage

### Encrypting Text

1. Enter the password
2. Type or paste the text to encrypt into the top panel
3. Click **🔒 Encrypt**
4. The encrypted text appears in the bottom panel
5. Save it (💾) or copy it (📋)

### Decrypting Text

1. Enter the same password used for encryption
2. Place the encrypted text in the bottom panel (open, paste, or drag & drop)
3. Click **🔓 Decrypt**
4. The original text appears in the top panel

### File Handling

**Open:**
- Click **📂 Open** or drag a file into the window (drag & drop)
- `.wtx` files automatically go to the ciphertext panel; if a password is set, they auto-decrypt

**Save:**
- Click **💾 Save** — the browser prompts for a filename
- Saving from the ciphertext panel automatically appends `.wtx` extension
- Saving from the plaintext panel accepts any name/extension

---

## 5. Advanced Features

### Drag & Drop

Drag any text file into the window. `.wtx` files go to the ciphertext panel, everything else goes to the plaintext panel. If a `.wtx` file is dropped and a password is set, it auto-decrypts.

### Insert / Overwrite Mode

Press **Insert** to toggle between insert and overwrite mode. The status bar shows the current mode (`INS` or `OVR`).

### Tab Key

The Tab key inserts 3 spaces (instead of moving focus).

### Copy All

The **📋 Copy All** button copies both panels to the clipboard with headers — useful when you want to send both the original and encrypted text.

---

## 6. File Formats

| Extension | Content |
|-----------|---------|
| `.txt` | Any text file (UTF-8 encoded) |
| `.wtx` | WERTHEIM-encrypted file (Base64 text) |

`.wtx` files are compatible with the original Pascal program's format.

---

## 7. Security Notes

- **Don't forget your password** — without it, the encrypted text is permanently unreadable
- WERTHEIM's encryption strength depends on password length and complexity
- Use long, random passwords for best security
- The app does not store passwords or send data over the network

---

## 8. Troubleshooting

| Problem | Solution |
|---------|----------|
| Buttons are disabled | Enter a password |
| Decrypting gives garbled text | Wrong password — an incorrect password produces garbage output |
| Save does nothing | Your browser may block downloads — allow them |
| Drag & drop doesn't work | Use the **📂 Open** button as an alternative |
| Characters are garbled | Make sure you're opening a plain text file (not `.pdf`, `.docx`) |
