# WERTHEIM — Manuel d'utilisation

## 1. Introduction

**WERTHEIM** est une application de chiffrement de texte fonctionnant dans le navigateur. Elle permet de chiffrer n'importe quel texte avec un mot de passe, de sauvegarder le texte chiffré dans un fichier, de l'envoyer, et le destinataire peut le déchiffrer avec le bon mot de passe.

**Important :** Tout le chiffrement et déchiffrement s'effectue sur votre machine. Rien ne quitte votre navigateur — pas de serveur, pas de téléchargement de données.

---

## 2. Installation

**Aucune installation requise.** Il suffit d'ouvrir un seul fichier HTML :

1. Téléchargez le fichier `wertheim.html`
2. Double-cliquez dessus — il s'ouvre dans votre navigateur
3. L'application est immédiatement prête à l'emploi

**Navigateurs pris en charge :** Chrome, Brave, Edge, Firefox, Safari, Opera — tous les navigateurs modernes.

---

## 3. Aperçu de l'interface

```
┌─────────────────────────────────────┐
│          🔐 WERTHEIM                │
│  Chiffrement de texte               │
├─────────────────────────────────────┤
│  [Mot de passe]             [👁]    │
├─────────────────────────────────────┤
│  [🔒 Chiffrer] [🔓 Déchiffrer]      │
│  [⇅ Échanger] [📋 Tout copier]      │
│  [🗑 Nouveau / Effacer]             │
├─ Panneau "Texte" ────────────────────┤
│  [📂 Ouvrir] [💾 Enregistrer]        │
│  [📋 Copier] [📋 Coller]            │
│  [🗑 Effacer]                       │
│  ┌────────────────────────────────┐ │
│  │ Saisissez le texte à          │ │
│  │ chiffrer...                    │ │
│  └────────────────────────────────┘ │
├─ Panneau "Chiffré" ──────────────────┤
│  [📂 Ouvrir] [💾 Enregistrer]        │
│  [📋 Copier] [📋 Coller]            │
│  [🗑 Effacer]                       │
│  ┌────────────────────────────────┐ │
│  │ ...texte chiffré affiché ici   │ │
│  └────────────────────────────────┘ │
├─────────────────────────────────────┤
│  INS        Ligne : 1  Col. : 1     │
└─────────────────────────────────────┘
```

### Éléments principaux :

| Élément | Description |
|---------|-------------|
| **Mot de passe** | Saisissez le mot de passe ici. Sans mot de passe, les boutons sont inactifs. |
| **👁** | Afficher/masquer le mot de passe |
| **🔒 Chiffrer** | Chiffre le texte du panneau supérieur et place le résultat dans le panneau inférieur |
| **🔓 Déchiffrer** | Déchiffre le texte du panneau inférieur vers le panneau supérieur |
| **⇅ Échanger** | Échange le contenu des deux panneaux |
| **📋 Tout copier** | Copie le contenu des deux panneaux dans le presse-papiers (avec en-têtes) |
| **🗑 Nouveau / Effacer** | Efface le contenu des deux panneaux (après confirmation) |

### Barre d'outils des panneaux (identique pour les deux) :

| Bouton | Description |
|--------|-------------|
| **📂 Ouvrir** | Charger un fichier depuis l'ordinateur |
| **💾 Enregistrer** | Sauvegarder le contenu du panneau dans un fichier |
| **📋 Copier** | Copier le contenu du panneau dans le presse-papiers |
| **📋 Coller** | Coller le contenu du presse-papiers dans le panneau (à la position du curseur) |
| **🗑 Effacer** | Effacer le contenu du panneau |

### Barre d'état :

| Élément | Description |
|---------|-------------|
| **INS / OVR** | Mode Insertion (INS) / Réécriture (OVR) — bascule avec la touche **Inser** |
| **Ligne : / Col. :** | Position actuelle du curseur |
| **🧪 Tests** | Exécute les tests automatiques (le résultat s'affiche 8 secondes) |

---

## 4. Utilisation de base

### Chiffrer un texte

1. Saisissez le mot de passe dans le champ prévu
2. Dans le panneau supérieur, saisissez (ou collez) le texte à chiffrer
3. Cliquez sur **🔒 Chiffrer**
4. Le texte chiffré apparaît dans le panneau inférieur
5. Vous pouvez l'enregistrer (💾) ou le copier (📋)

### Déchiffrer un texte

1. Saisissez le mot de passe (le même que celui utilisé pour le chiffrement)
2. Placez le texte chiffré dans le panneau inférieur (ouvrir, coller ou glisser-déposer)
3. Cliquez sur **🔓 Déchiffrer**
4. Le texte original apparaît dans le panneau supérieur

### Gestion des fichiers

**Ouvrir :**
- Cliquez sur **📂 Ouvrir** ou glissez-déposez le fichier dans la fenêtre
- Les fichiers `.wtx` sont automatiquement placés dans le panneau chiffré, et si le mot de passe est fourni, ils sont déchiffrés immédiatement

**Enregistrer :**
- Le bouton **💾 Enregistrer** demande un nom de fichier
- Depuis le panneau chiffré, l'extension `.wtx` est ajoutée automatiquement
- Depuis le panneau texte, n'importe quel nom/extension peut être utilisé

---

## 5. Fonctionnalités avancées

### Glisser-déposer (drag & drop)

Vous pouvez glisser-déposer n'importe quel fichier texte dans la fenêtre. Les fichiers `.wtx` sont placés dans le panneau chiffré, tous les autres dans le panneau texte. Si vous déposez un fichier `.wtx` et que le mot de passe est déjà saisi, le déchiffrement s'effectue automatiquement.

### Mode Insertion / Réécriture

La touche **Inser** permet de basculer entre le mode insertion et le mode réécriture. La barre d'état indique le mode actuel (`INS` ou `OVR`).

### Touche Tabulation

La touche Tab insère 3 espaces (au lieu de déplacer le focus).

### Tout copier

Le bouton **📋 Tout copier** place le contenu des deux panneaux dans le presse-papiers, avec leurs en-têtes — utile pour envoyer à la fois le texte clair et sa version chiffrée.

---

## 6. Formats de fichiers

| Extension | Contenu |
|-----------|---------|
| `.txt` | Tout fichier texte (encodage UTF-8) |
| `.wtx` | Fichier chiffré avec WERTHEIM (texte Base64) |

Les fichiers `.wtx` sont compatibles avec le format utilisé par le programme Pascal d'origine.

---

## 7. Consignes de sécurité

- **N'oubliez pas le mot de passe** — sans lui, le texte chiffré est définitivement illisible
- La robustesse du chiffrement WERTHEIM dépend de la longueur et de la complexité du mot de passe
- Il est recommandé d'utiliser des mots de passe longs et aléatoires
- L'application ne stocke aucun mot de passe et n'envoie aucune donnée sur le réseau

---

## 8. Dépannage

| Problème | Solution |
|----------|----------|
| Les boutons sont inactifs | Saisissez le mot de passe |
| Le déchiffrement ne donne pas le texte original | Mauvais mot de passe — un mot de passe incorrect produit un texte incohérent |
| Rien ne se passe lors de l'enregistrement | Le navigateur peut bloquer le téléchargement — autorisez-le |
| Le glisser-déposer ne fonctionne pas | Utilisez le bouton **📂 Ouvrir** comme alternative |
| Caractères déformés | Assurez-vous d'ouvrir uniquement des fichiers texte (pas de `.pdf`, `.docx`) |
