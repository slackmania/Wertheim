# WERTHEIM titkosító — Felhasználói kézikönyv

## 1. Bevezetés

A **WERTHEIM** egy böngészőben futó szövegtitkosító alkalmazás. Segítségével bármilyen szöveget titkosíthatsz jelszóval, majd a titkosított szöveget fájlba mentheted, elküldheted, és a címzett a megfelelő jelszó birtokában visszafejtheti.

**Fontos:** Minden titkosítás és visszafejtés a te gépeden történik. Semmi sem hagyja el a böngésződet, nincs szerver, nincs adatfeltöltés.

---

## 2. Telepítés

**Nincs telepítés.** Egyetlen HTML fájlt kell megnyitni:

1. Töltsd le a `wertheim.html` fájlt
2. Kattints rá duplán — megnyílik a böngésződben
3. Az alkalmazás azonnal használatra kész

**Támogatott böngészők:** Chrome, Brave, Edge, Firefox, Safari, Opera — minden modern böngésző.

---

## 3. Felület áttekintése

```
┌─────────────────────────────────────┐
│          🔐 WERTHEIM                │
│  Szöveg titkosító                    │
├─────────────────────────────────────┤
│  [Jelszó mező]              [👁]    │
├─────────────────────────────────────┤
│  [🔒 Titkosítás] [🔓 Visszafejtés]  │
│  [⇅ Csere] [📋 Mindent másol]      │
│  [🗑 Új / Töröl]                    │
├─ "Szöveg" panel ────────────────────┤
│  [📂 Megnyitás] [💾 Mentés]         │
│  [📋 Másolás] [📋 Beillesztés]      │
│  [🗑 Törlés]                        │
│  ┌────────────────────────────────┐ │
│  │ Ide írd a titkosítandó        │ │
│  │ szöveget...                    │ │
│  └────────────────────────────────┘ │
├─ "Titkosított" panel ───────────────┤
│  [📂 Megnyitás] [💾 Mentés]         │
│  [📋 Másolás] [📋 Beillesztés]      │
│  [🗑 Törlés]                        │
│  ┌────────────────────────────────┐ │
│  │ ...itt jelenik meg a           │ │
│  │ titkosított szöveg...          │ │
│  └────────────────────────────────┘ │
├─────────────────────────────────────┤
│  INS          Sor: 1  Oszlop: 1     │
└─────────────────────────────────────┘
```

### Fő elemek:

| Elem | Leírás |
|------|--------|
| **Jelszó mező** | Ide kell beírni a jelszót. Enélkül a gombok inaktívak. |
| **👁** | Jelszó megjelenítése/elrejtése |
| **🔒 Titkosítás** | A felső panel szövegét titkosítja és az eredményt az alsó panelbe teszi |
| **🔓 Visszafejtés** | Az alsó panel titkosított szövegét fejti vissza a felső panelbe |
| **⇅ Csere** | A két panel tartalmát felcseréli |
| **📋 Mindent másol** | Mindkét panel tartalmát a vágólapra másolja (címsorokkal együtt) |
| **🗑 Új / Töröl** | Mindkét panel tartalmát törli (megerősítés után) |

### Panel eszköztár (mindkét panelen):

| Gomb | Leírás |
|------|--------|
| **📂 Megnyitás** | Fájl betöltése a számítógépről |
| **💾 Mentés** | Panel tartalmának mentése fájlba (fájlnév megadásával) |
| **📋 Másolás** | Panel tartalmának másolása a vágólapra |
| **📋 Beillesztés** | Vágólap tartalmának beillesztése a panelbe (kurzor pozícióba) |
| **🗑 Törlés** | Panel tartalmának törlése |

### Állapotsor:

| Elem | Leírás |
|------|--------|
| **INS / OVR** | Beszúrás mód (INS) / Felülírás mód (OVR) — váltás **Insert** billentyűvel |
| **Sor: / Oszlop:** | Kurzor aktuális pozíciója |
| **🧪 Tesztek** | Automatikus tesztek futtatása (az eredmény 8 másodpercig látszik) |

---

## 4. Alap használat

### Szöveg titkosítása

1. Írd be a jelszót a jelszó mezőbe
2. A felső panelbe írd be (vagy illeszd be) a titkosítandó szöveget
3. Kattints a **🔒 Titkosítás** gombra
4. A titkosított szöveg megjelenik az alsó panelben
5. Ezt elmentheted (💾), vagy kimásolhatod (📋)

### Szöveg visszafejtése

1. Írd be a jelszót (ugyanazt, amivel titkosítottad)
2. Az alsó panelbe tedd a titkosított szöveget (megnyitás, beillesztés, vagy behúzás)
3. Kattints a **🔓 Visszafejtés** gombra
4. Az eredeti szöveg megjelenik a felső panelben

### Fájlok kezelése

**Megnyitás:**
- Kattints a **📂 Megnyitás** gombra, vagy húzd a fájlt az ablakba (drag & drop)
- `.wtx` fájlok automatikusan a titkosított panelbe kerülnek, és ha van jelszó, rögtön ki is fejtődnek

**Mentés:**
- A **💾 Mentés** gombra kattintva a böngésző fájlnevet kérdez
- Titkosított panelből mentve a `.wtx` kiterjesztés automatikusan hozzáadódik
- Szöveg panelből mentve bármilyen név/névkiterjesztés megadható

---

## 5. Haladó funkciók

### Drag & Drop (fájl behúzás)

Bármilyen szövegfájlt behúzhatsz az ablakba. A `.wtx` fájlok a titkosított panelbe, minden más a szöveg panelbe kerül. Ha `.wtx` fájlt húzol be és a jelszó meg van adva, automatikusan visszafejti.

### Insert / Overwrite mód

Az **Insert** billentyűvel válthatsz a beszúrás és felülírás között. Az állapotsor mutatja az aktuális módot (`INS` vagy `OVR`).

### Tab billentyű

A Tab billentyű 3 szóközt szúr be (nem a fókusz ugrik tovább).

### Másolás mindkét panelből

A **📋 Mindent másol** gomb mindkét panel tartalmát a vágólapra teszi, címsorokkal ellátva — hasznos, ha mind a szöveget, mind a titkosított változatot szeretnéd elküldeni.

---

## 6. Fájl formátumok

| Kiterjesztés | Tartalom |
|-------------|----------|
| `.txt` | Bármilyen szövegfájl (UTF-8 kódolás) |
| `.wtx` | WERTHEIM-mel titkosított fájl (Base64 szöveg) |

A `.wtx` fájlok kompatibilisek az eredeti Pascal program által használt formátummal.

---

## 7. Biztonsági tudnivalók

- A jelszót **ne felejtsd el** — ha elfelejted, a titkosított szöveg örökre olvashatatlan
- A WERTHEIM titkosítási erőssége a jelszó hosszától és összetettségétől függ
- Hosszú, véletlenszerű jelszavak használata ajánlott
- Az alkalmazás nem tárol jelszavakat, nem küld adatot a hálózaton

---

## 8. Hibaelhárítás

| Probléma | Megoldás |
|----------|----------|
| A gombok inaktívak | Írd be a jelszót |
| Visszafejtésnél nem az eredeti szöveg jön | Rossz jelszó — a helytelen jelszó összefüggéstelen szöveget ad |
| Fájl mentésnél nem történik semmi | A böngésző blokkolhatja a letöltést — engedélyezd |
| Nincs drag & drop | Használd a **📂 Megnyitás** gombot alternatívaként |
| Karakterek elcsúszva | Győződj meg róla, hogy csak szövegfájlt nyitsz meg (ne `.pdf`-et, `.docx`-et) |
