# WERTHEIM — Documentation technique

## Aperçu

**WERTHEIM** est une application de chiffrement autonome sous forme d'un seul fichier HTML, exécutée dans le navigateur. Elle a été portée du Pascal/Delphi original vers JavaScript via TypeScript. Objectif : offrir une interface moderne sans installation tout en maintenant la compatibilité totale avec le format de fichier `.wtx`.

---

## Pipeline de chiffrement

### Encodage (`hintabe()`)

```
texte clair (string)
    │
    ▼
Encodage UTF-8 → Uint8Array
    │
    ▼
RC4 + SHA-1 (hash du mot de passe)
    │
    ▼
Chaîne latin1 (octet→caractère, 0..255)
    │
    ▼
encode64() — Base64 interne
    │
    ▼
bekod() — substitution hinta (2 passes) → encode64() — Base64 externe
    │
    ▼
texte chiffré (string)
```

### Décodage (`hintaki()`)

```
texte chiffré (string)
    │
    ▼
kikod() — decode64() → déchiffrement hinta (2 passes) → decode64()
    │
    ▼
Chaîne latin1
    │
    ▼
charCodeAt(i) & 0xFF → Uint8Array
    │
    ▼
RC4 + SHA-1 (hash du mot de passe) (déchiffrement = rechiffrement)
    │
    ▼
Décodage UTF-8
    │
    ▼
texte clair (string)
```

---

## Composants détaillés

### 1. SHA-1 — Web Crypto API

```javascript
async function sha1(str) {
    const buf = new TextEncoder().encode(str);
    const hash = await crypto.subtle.digest("SHA-1", buf);
    return new Uint8Array(hash);
}
```

Utilise l'implémentation SHA-1 native et accélérée matériellement du navigateur. Produit un hash de 20 octets à partir du mot de passe, nécessaire à la génération de la clé RC4.

### 2. Chiffrement RC4

Implémentation JavaScript native suivant l'algorithme de `titokrutin.pas` (Pascal). L'ordonnancement de clé (KSA) et le générateur pseudo-aléatoire (PRNG) sont du RC4 standard. Le déchiffrement est identique au chiffrement (propriété XOR).

### 3. Conversion d'octets

Le composant le plus critique. La sortie de RC4 est une séquence d'octets (0–255). Pour la traiter comme Base64, les octets doivent être convertis en caractères :

**Approche correcte :**
```javascript
String.fromCharCode(...enc)  // octet → caractère (0–255 → U+0000–U+00FF)
```

**Approche incorrecte (version précédente) :**
```javascript
new TextDecoder("latin1").decode(enc)  // 0x80–0x9F → multi-octets Windows-1252
```

`TextDecoder("latin1")` convertit la plage 0x80–0x9F en caractères Windows-1252 (ex. 0x80 → € = U+20AC), ce qui brise le codage Base64 car les points de code résultants dépassent 255. `String.fromCharCode()` mappe chaque octet 1:1 sans modification.

### 4. Codage Base64 (`encode64` / `decode64`)

Base64 non standard — compatible avec l'original Pascal. L'alphabet ABC0 (`"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"`) est identique à la version Pascal. Le codage transforme 3 octets en 4 caractères, le remplissage se fait avec le caractère `=`.

### 5. Substitution Hinta

**Concept :** Chaque caractère du mot de passe correspond à une ligne de la table de permutation (`tarr`). La `i`-ème ligne contient une permutation de tous les caractères de ABC0. Chaque caractère du texte est substitué par le caractère situé à la ligne et colonne correspondantes.

**Implémentation :**

```javascript
function genhinta(pwd) {
    const pwdenc = encode64(pwd);              // → Base64
    const trimmed = pwdenc.replace(/=+$/, "");  // padding supprimé
    const kod = new Array(MAX_PWD_LEN);        // indices de ligne
    for (let t = 0; t < pwdhossz && t < MAX_PWD_LEN; t++) {
        kod[t] = pwdenc.charCodeAt(t) % TARR_ROWS;
        hintar[t] = tarr[kod[t]][col];
    }
}
```

**Remarque importante :** L'indice `col` utilisé ici est `pwd.charCodeAt(t)`, où `pwd` est `encode64(password)`. Les points de code des caractères Base64 se situent entre 43 et 122, mais les tableaux `tarr[row]` ne contiennent que 65 indices (0–64). Ainsi, `hintar[t]` sera `undefined` pour la plupart des mots de passe (dans l'original Pascal, c'est un comportement indéfini, mais il est identique). Cela **n'affecte pas la validité du round-trip** car l'indice erroné se manifeste symétriquement dans le chiffrement et le déchiffrement.

**Rôle de hinta :** La substitution en 2 passes ajoute un brassage supplémentaire à la sortie Base64. Bien qu'en raison de l'erreur d'indice de colonne ci-dessus, l'implémentation actuelle avec la plupart des mots de passe ne modifie pratiquement pas le texte Base64 (la substitution remplace les caractères par une chaîne vide), ce comportement est symétrique, donc le déchiffrement restaure correctement le texte.

### 6. Table `tarr`

Table de permutation de taille 64×65. Chaque ligne contient une permutation aléatoire de tous les caractères de ABC0. Générée par le programme Pascal et intégrée en dur dans le code source (`tarr_data.ts` → `wertheim.html`).

---

## Architecture de l'interface

### Structure HTML

```
.app
├── h1 + titre
├── .pw-row (mot de passe + bouton afficher/masquer)
├── .action-row (Chiffrer, Déchiffrer, Échanger, Tout copier, Nouveau/Effacer)
├── h2 + .toolbar + .editor-panel (Texte)
├── h2 + .toolbar + .editor-panel (Chiffré)
├── .status-bar (INS/OVR, position, tests)
├── .footer
└── .drop-overlay (glisser-déposer)
```

### Modules JavaScript

| Fonction | Description |
|----------|-------------|
| `sha1()` | Hash SHA-1 (Web Crypto API) |
| `rc4()` | Chiffrement/déchiffrement RC4 |
| `besecure()` | Chiffrement RC4 + conversion octet→caractère |
| `kisecure()` | Conversion caractère→octet + RC4 |
| `encode64()` / `decode64()` | Base64 (compatible Pascal) |
| `genhinta()` | Génération de la table de permutation Hinta depuis le mot de passe |
| `bekod()` / `kikod()` | Substitution Hinta (2 passes) |
| `hintabe()` / `hintaki()` | Chaîne complète de chiffrement/déchiffrement |
| `updateButtons()` | Surveillance du champ mot de passe — activation/désactivation des boutons |
| `saveFile()` | Sauvegarde de fichier avec dialogue de confirmation |
| `copyText()` / `copyAll()` | Opérations presse-papiers |
| `pasteText()` | Collage à la position du curseur |
| `updatePos()` | Suivi de la position du curseur |

### Style

- Palette sombre : fond `#1a1a2e`, accent `#e94560`
- Responsive : `max-width: 960px`, centré
- Éditeurs : hauteur flexible (`flex: 1`), textarea défilante
- États des boutons : visuellement distincts (normal, survol, désactivé)

---

## Analyse de sécurité

### Faiblesses

1. **Erreur d'indice de colonne Hinta :** L'indice `pwd.charCodeAt(t)` dans `genhinta()` dépasse la taille du tableau `tarr[row]` pour la plupart des mots de passe, ce qui désactive pratiquement la substitution Hinta. L'erreur étant symétrique, la compatibilité n'est pas compromise, mais la robustesse du chiffrement est réduite.

2. **Faiblesses connues de RC4 :** RC4 n'est plus considéré comme un standard moderne. Biais dans les premiers octets et attaques statistiques documentées.

3. **Absence d'authentification :** L'intégrité du message n'est pas protégée. Le texte chiffré peut être modifié sans détection.

### Forces

1. **Conversion d'octets fidèle :** `String.fromCharCode()` garantit un mappage 1:1 de chaque octet, contrairement au comportement erroné de `TextDecoder("latin1")`.

2. **Aucune dépendance externe :** Un seul fichier, pas de CDN, pas de code tiers.

3. **Conception zero-trust :** Pas de serveur, pas d'envoi de données, pas de télémétrie.

---

## Fichiers et construction

### Fichiers

| Fichier | Description |
|---------|-------------|
| `wertheim.html` | L'application finale (à utiliser) |
| `src/wertheim.ts` | Code source TypeScript |
| `src/tarr_data.ts` | Données de la table Tarr |
| `src/index.ts` | Point d'entrée de l'interface |
| `test/*.ts` | Tests |
| `hinta.pas` | Original Pascal (substitution Hinta + Base64) |
| `titokrutin.pas` | Original Pascal (RC4 + SHA-1) |
| `unit1.pas` | Original Pascal (GUI) |

### Construction

```bash
npm install
npm run build    # → dist/wertheim.js
```

Le HTML final a été assemblé manuellement en fusionnant la sortie TypeScript et le code d'interface écrit à la main.

### Tests

```bash
npm test
```

Les tests couvrent :
- Round-trip Base64
- Chiffrement/déchiffrement de base
- Caractères UTF-8 accentués
- Texte long (1000 caractères)
- Mot de passe incorrect (vérification du texte altéré)

---

## Historique des versions

| Version | Date | Changement |
|---------|------|------------|
| 1.0 | — | Original Pascal |
| 2.0 | — | Portage TypeScript |
| 2.1 | — | Correction du bug `TextDecoder("latin1")` → `String.fromCharCode()` |
| 2.2 | — | Refonte complète de l'interface : gestion de fichiers, glisser-déposer, presse-papiers, fonctions d'édition |
