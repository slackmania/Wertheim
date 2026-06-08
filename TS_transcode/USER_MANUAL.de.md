# WERTHEIM Verschlüsselung — Benutzerhandbuch

## 1. Einführung

**WERTHEIM** ist eine browserbasierte Textverschlüsselungsanwendung. Damit kannst du jeden beliebigen Text mit einem Passwort verschlüsseln, die verschlüsselte Textdatei speichern und versenden – der Empfänger entschlüsselt sie mit dem richtigen Passwort.

**Wichtig:** Alle Ver- und Entschlüsselungen erfolgen ausschließlich auf deinem Gerät. Nichts verlässt deinen Browser – kein Server, kein Daten-Upload.

---

## 2. Installation

**Keine Installation erforderlich.** Es genügt, eine einzige HTML-Datei zu öffnen:

1. Lade die Datei `wertheim.html` herunter
2. Doppelklicke darauf – sie öffnet sich in deinem Browser
3. Die Anwendung ist sofort einsatzbereit

**Unterstützte Browser:** Chrome, Brave, Edge, Firefox, Safari, Opera – alle modernen Browser.

---

## 3. Benutzeroberfläche

```
┌─────────────────────────────────────┐
│          🔐 WERTHEIM                │
│  Textverschlüsselung                 │
├─────────────────────────────────────┤
│  [Passwortfeld]             [👁]    │
├─────────────────────────────────────┤
│  [🔒 Verschlüsseln] [🔓 Entschl.]  │
│  [⇅ Tauschen] [📋 Alles kopieren]  │
│  [🗑 Neu / Löschen]                │
├─ "Text"-Panel ──────────────────────┤
│  [📂 Öffnen] [💾 Speichern]        │
│  [📋 Kopieren] [📋 Einfügen]       │
│  [🗑 Löschen]                      │
│  ┌────────────────────────────────┐ │
│  │ Hier den zu verschlüsselnden  │ │
│  │ Text eingeben...               │ │
│  └────────────────────────────────┘ │
├─ "Verschlüsselt"-Panel ─────────────┤
│  [📂 Öffnen] [💾 Speichern]        │
│  [📋 Kopieren] [📋 Einfügen]       │
│  [🗑 Löschen]                      │
│  ┌────────────────────────────────┐ │
│  │ ...hier erscheint der          │ │
│  │ verschlüsselte Text...         │ │
│  └────────────────────────────────┘ │
├─────────────────────────────────────┤
│  INS         Zeile: 1  Spalte: 1    │
└─────────────────────────────────────┘
```

### Hauptelemente

| Element | Beschreibung |
|---------|--------------|
| **Passwortfeld** | Hier wird das Passwort eingegeben. Ohne Passwort sind die Schaltflächen inaktiv. |
| **👁** | Passwort ein-/ausblenden |
| **🔒 Verschlüsseln** | Verschlüsselt den Text im oberen Panel und schreibt das Ergebnis ins untere Panel |
| **🔓 Entschlüsseln** | Entschlüsselt den Text im unteren Panel und schreibt das Ergebnis ins obere Panel |
| **⇅ Tauschen** | Vertauscht die Inhalte beider Panels |
| **📋 Alles kopieren** | Kopiert beide Panelinhalte (mit Überschriften) in die Zwischenablage |
| **🗑 Neu / Löschen** | Löscht beide Panelinhalte (nach Bestätigung) |

### Panel-Symbolleiste (beide Panels)

| Schaltfläche | Beschreibung |
|-------------|--------------|
| **📂 Öffnen** | Lädt eine Datei vom Computer |
| **💾 Speichern** | Speichert den Panel-Inhalt in eine Datei (mit Namensabfrage) |
| **📋 Kopieren** | Kopiert den Panel-Inhalt in die Zwischenablage |
| **📋 Einfügen** | Fügt den Inhalt der Zwischenablage an der Cursorposition ein |
| **🗑 Löschen** | Löscht den Panel-Inhalt |

### Statusleiste

| Element | Beschreibung |
|---------|--------------|
| **INS / OVR** | Einfügemodus (INS) / Überschreibmodus (OVR) – Umschalten mit der **Einfg**-Taste |
| **Zeile: / Spalte:** | Aktuelle Cursorposition |
| **🧪 Tests** | Automatische Tests ausführen (Ergebnis wird 8 Sekunden lang angezeigt) |

---

## 4. Grundlegende Nutzung

### Text verschlüsseln

1. Gib das Passwort in das Passwortfeld ein
2. Gib (oder füge) den zu verschlüsselnden Text in das obere Panel ein
3. Klicke auf **🔒 Verschlüsseln**
4. Der verschlüsselte Text erscheint im unteren Panel
5. Du kannst ihn speichern (💾) oder kopieren (📋)

### Text entschlüsseln

1. Gib das Passwort ein (dasselbe, mit dem verschlüsselt wurde)
2. Füge den verschlüsselten Text in das untere Panel ein (öffnen, einfügen oder ziehen)
3. Klicke auf **🔓 Entschlüsseln**
4. Der ursprüngliche Text erscheint im oberen Panel

### Dateiverwaltung

**Öffnen:**
- Klicke auf **📂 Öffnen** oder ziehe eine Datei per Drag & Drop ins Fenster
- `.wtx`-Dateien werden automatisch in das verschlüsselte Panel geladen und bei vorhandenem Passwort sofort entschlüsselt

**Speichern:**
- Ein Klick auf **💾 Speichern** öffnet einen Dialog zur Namensvergabe
- Beim Speichern aus dem verschlüsselten Panel wird automatisch die Endung `.wtx` angehängt
- Beim Speichern aus dem Text-Panel kann ein beliebiger Name mit beliebiger Endung vergeben werden

---

## 5. Erweiterte Funktionen

### Drag & Drop (Datei ziehen)

Du kannst beliebige Textdateien in das Fenster ziehen. `.wtx`-Dateien landen im verschlüsselten Panel, alle anderen im Text-Panel. Wenn eine `.wtx`-Datei gezogen wird und das Passwort bereits eingegeben ist, erfolgt die Entschlüsselung automatisch.

### Einfügen / Überschreiben (INS/OVR)

Mit der **Einfg**-Taste (Insert) schaltest du zwischen dem Einfüge- und Überschreibmodus um. Die Statusleiste zeigt den aktuellen Modus an (`INS` oder `OVR`).

### Tab-Taste

Die Tab-Taste fügt 3 Leerzeichen ein (der Fokus springt nicht zum nächsten Element).

### Alles kopieren

Die Schaltfläche **📋 Alles kopieren** kopiert die Inhalte beider Panels mit Überschriften in die Zwischenablage – nützlich, wenn du sowohl den Klartext als auch die verschlüsselte Version versenden möchtest.

---

## 6. Dateiformate

| Endung | Inhalt |
|--------|--------|
| `.txt` | Beliebige Textdatei (UTF-8-Kodierung) |
| `.wtx` | Mit WERTHEIM verschlüsselte Datei (Base64-Text) |

Die `.wtx`-Dateien sind kompatibel mit dem Format des ursprünglichen Pascal-Programms.

---

## 7. Sicherheitshinweise

- **Vergiss dein Passwort nicht** – ohne das Passwort ist der verschlüsselte Text für immer unlesbar
- Die Sicherheit von WERTHEIM hängt von der Länge und Komplexität des Passworts ab
- Die Verwendung langer, zufälliger Passwörter wird empfohlen
- Die Anwendung speichert keine Passwörter und sendet keine Daten über das Netzwerk

---

## 8. Fehlerbehebung

| Problem | Lösung |
|---------|--------|
| Schaltflächen sind inaktiv | Passwort eingeben |
| Beim Entschlüsseln erscheint nicht der Originaltext | Falsches Passwort – ein falsches Passwort ergibt sinnlosen Text |
| Beim Speichern passiert nichts | Der Browser blockiert möglicherweise den Download – erlaube ihn |
| Drag & Drop funktioniert nicht | Verwende alternativ die Schaltfläche **📂 Öffnen** |
| Zeichen sind verschoben | Stelle sicher, dass du nur Textdateien öffnest (keine `.pdf` oder `.docx`) |
